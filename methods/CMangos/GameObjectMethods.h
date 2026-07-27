/*
* Copyright (C) 2010 - 2024 Eluna Lua Engine <https://elunaluaengine.github.io/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

#ifndef GAMEOBJECTMETHODS_H
#define GAMEOBJECTMETHODS_H

/***
 * Inherits all methods from: [Object], [WorldObject]
 */
namespace LuaGameObject
{
    /**
     * Returns 'true' if the [GameObject] can give the specified [Quest]
     *
     * @param uint32 questId : quest entry Id to check
     * @return bool hasQuest
     */
    int HasQuest(Eluna* E, GameObject* go)
    {
        uint32 questId = E->CHECKVAL<uint32>(2);

        E->Push(go->HasQuest(questId));
        return 1;
    }

    /**
     * Returns 'true' if the [GameObject] is spawned
     *
     * @return bool isSpawned
     */
    int IsSpawned(Eluna* E, GameObject* go)
    {
        E->Push(go->IsSpawned());
        return 1;
    }

    /**
     * Returns 'true' if the [GameObject] is a transport
     *
     * @return bool isTransport
     */
    int IsTransport(Eluna* E, GameObject* go)
    {
        E->Push(go->IsTransport());
        return 1;
    }

    /**
     * Returns 'true' if the [GameObject] is active
     *
     * @return bool isActive
     */
    int IsActive(Eluna* E, GameObject* go)
    {
        E->Push(go->isActiveObject());
        return 1;
    }

    /**
     * Returns display ID of the [GameObject]
     *
     * @return uint32 displayId
     */
    int GetDisplayId(Eluna* E, GameObject* go)
    {
        E->Push(go->GetDisplayId());
        return 1;
    }

    /**
     * Returns the state of a [GameObject]
     * Below are client side [GOState]s off of 3.3.5a
     *
     * <pre>
     * enum GOState
     * {
     *     GO_STATE_ACTIVE             = 0,                        // show in world as used and not reset (closed door open)
     *     GO_STATE_READY              = 1,                        // show in world as ready (closed door close)
     *     GO_STATE_ACTIVE_ALTERNATIVE = 2                         // show in world as used in alt way and not reset (closed door open by cannon fire)
     * };
     * </pre>
     *
     * @return [GOState] goState
     */
    int GetGoState(Eluna* E, GameObject* go)
    {
        E->Push(go->GetGoState());
        return 1;
    }

    /**
     * Returns the [LootState] of a [GameObject]
     * Below are [LootState]s off of 3.3.5a
     *
     * <pre>
     * enum LootState
     * {
     *     GO_NOT_READY = 0,
     *     GO_READY,                                               // can be ready but despawned, and then not possible activate until spawn
     *     GO_ACTIVATED,
     *     GO_JUST_DEACTIVATED
     * };
     * </pre>
     *
     * @return [LootState] lootState
     */
    int GetLootState(Eluna* E, GameObject* go)
    {
        E->Push(go->GetLootState());
        return 1;
    }

    /**
     * Returns the [Player] that can loot the [GameObject]
     *
     * Not the original looter and may be nil.
     *
     * @return [Player] player
     */
    int GetLootRecipient(Eluna* E, GameObject* go)
    {
        E->Push(go->GetLootRecipient());
        return 1;
    }

    /**
     * Returns the [Group] that can loot the [GameObject]
     *
     * Not the original looter and may be nil.
     *
     * @return [Group] group
     */
    int GetLootRecipientGroup(Eluna* E, GameObject* go)
    {
        E->Push(go->GetGroupLootRecipient());
        return 1;
    }

    /**
     * Returns the guid of the [GameObject] that is used as the ID in the database
     *
     * @return uint32 dbguid
     */
    int GetDBTableGUIDLow(Eluna* E, GameObject* go)
    {
        // on mangos based this is same as lowguid
        E->Push(go->GetGUIDLow());
        return 1;
    }

    /**
     * Sets the state of a [GameObject]
     *
     * <pre>
     * enum GOState
     * {
     *     GO_STATE_ACTIVE             = 0,                        // show in world as used and not reset (closed door open)
     *     GO_STATE_READY              = 1,                        // show in world as ready (closed door close)
     *     GO_STATE_ACTIVE_ALTERNATIVE = 2                         // show in world as used in alt way and not reset (closed door open by cannon fire)
     * };
     * </pre>
     *
     * @param [GOState] state : all available go states can be seen above
     */
    int SetGoState(Eluna* E, GameObject* go)
    {
        uint32 state = E->CHECKVAL<uint32>(2, 0);

        if (state == 0)
            go->SetGoState(GO_STATE_ACTIVE);
        else if (state == 1)
            go->SetGoState(GO_STATE_READY);
        else if (state == 2)
            go->SetGoState(GO_STATE_ACTIVE_ALTERNATIVE);

        return 0;
    }

    /**
     * Sets the [LootState] of a [GameObject]
     * Below are [LootState]s off of 3.3.5a
     *
     * <pre>
     * enum LootState
     * {
     *     GO_NOT_READY = 0,
     *     GO_READY,                                               // can be ready but despawned, and then not possible activate until spawn
     *     GO_ACTIVATED,
     *     GO_JUST_DEACTIVATED
     * };
     * </pre>
     *
     * @param [LootState] state : all available loot states can be seen above
     */
    int SetLootState(Eluna* E, GameObject* go)
    {
        uint32 state = E->CHECKVAL<uint32>(2, 0);

        if (state == 0)
            go->SetLootState(GO_NOT_READY);
        else if (state == 1)
            go->SetLootState(GO_READY);
        else if (state == 2)
            go->SetLootState(GO_ACTIVATED);
        else if (state == 3)
            go->SetLootState(GO_JUST_DEACTIVATED);

        return 0;
    }

    /**
     * Saves [GameObject] to the database
     *
     */
    int SaveToDB(Eluna* /*E*/, GameObject* go)
    {
        go->SaveToDB();
        return 0;
    }

    /**
     * Removes [GameObject] from the world
     *
     * The object is no longer reachable after this and it is not respawned.
     *
     * @param bool deleteFromDB : if true, it will delete the [GameObject] from the database
     */
    int RemoveFromWorld(Eluna* E, GameObject* go)
    {
        bool deldb = E->CHECKVAL<bool>(2, false);

        // cs_gobject.cpp copy paste
        ObjectGuid ownerGuid = go->GetOwnerGuid();
        if (ownerGuid)
        {
            Unit* owner = eObjectAccessor()GetUnit(*go, ownerGuid);
            if (!owner || !ownerGuid.IsPlayer())
                return 0;

            owner->RemoveGameObject(go, false);
        }

        if (deldb)
        {
            go->DeleteFromDB();
        }

        go->SetRespawnTime(0);
        go->Delete();

        return 0;
    }

    /**
     * Activates a door or a button/lever
     *
     * @param uint32 delay = 0 : cooldown time in seconds to restore the [GameObject] back to normal. 0 for infinite duration
     */
    int UseDoorOrButton(Eluna* E, GameObject* go)
    {
        uint32 delay = E->CHECKVAL<uint32>(2, 0);

        go->UseDoorOrButton(delay);
        return 0;
    }

    /**
     * Despawns a [GameObject]
     *
     * The gameobject may be automatically respawned by the core
     */
    int Despawn(Eluna* /*E*/, GameObject* go)
    {
        go->SetLootState(GO_JUST_DEACTIVATED);
        return 0;
    }

    /**
     * Respawns a [GameObject]
     */
    int Respawn(Eluna* /*E*/, GameObject* go)
    {
        go->Respawn();
        return 0;
    }

    /**
     * Sets the respawn or despawn time for the gameobject.
     *
     * Respawn time is also used as despawn time depending on gameobject settings
     *
     * @param int32 delay = 0 : cooldown time in seconds to respawn or despawn the object. 0 means never
     */
    int SetRespawnTime(Eluna* E, GameObject* go)
    {
        int32 respawn = E->CHECKVAL<int32>(2);

        go->SetRespawnTime(respawn);
        return 0;
    }

    /**
     * Sets whether or not the [GameObject] will be spawned by default
     *
     * Primarily used for temporary spawns.
     *
     * @param bool spawn = true : if false, it will prevent the [GameObject] from respawning by default
     */
    int SetSpawnedByDefault(Eluna* E, GameObject* go)
    {
        bool spawn = E->CHECKVAL<bool>(2, true);

        go->SetSpawnedByDefault(spawn);
        return 0;
    }

    /**
     * Re-sends this [GameObject]'s create block (destroy + create) to every
     * player that currently sees it.
     *
     * 3.3.5 clients never re-render an already-instantiated gameobject from
     * value updates - rotation and scale only take effect at create time -
     * so call this after writing transform fields (GAMEOBJECT_ROTATION,
     * OBJECT_FIELD_SCALE_X) to make the change visible in place.
     */
    int Refresh(Eluna* /*E*/, GameObject* go)
    {
        // guard: calling this on an object that is still being ADDED to the
        // map (e.g. from an ON_SPAWN hook during grid load at player login)
        // sends packets to mid-login sessions and segfaults the world
        if (!go->IsInWorld())
        {
            return 0;
        }
        Map::PlayerList const& players = go->GetMap()->GetPlayers();
        for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
        {
            if (Player* player = itr->getSource())
            {
                if (player->HaveAtClient(go))
                {
                    go->DestroyForPlayer(player);
                    go->SendCreateUpdateToPlayer(player);
                }
            }
        }
        return 0;
    }

    /**
     * Removes this [GameObject] from the world WITHOUT the client-side
     * despawn animation.
     *
     * [GameObject:RemoveFromWorld] routes through GameObject::Delete(),
     * which always broadcasts SMSG_GAMEOBJECT_DESPAWN_ANIM - a visible
     * fade-out. For fast remove+respawn cycles (the GOCatalog staging
     * ghost) that fade reads as a blink, so this does the same teardown
     * silently. Temp spawns only: it skips the pool bookkeeping that
     * GameObject::Delete() performs for pooled DB spawns.
     */
    int Vanish(Eluna* /*E*/, GameObject* go)
    {
        ObjectGuid ownerGuid = go->GetOwnerGuid();

        if (ownerGuid)
        {
            Unit* owner = eObjectAccessor()GetUnit(*go, ownerGuid);
            if (!owner || !ownerGuid.IsPlayer())
                return 0;

            owner->RemoveGameObject(go, false);
        }

        go->SetRespawnTime(0);
        go->SetGoState(GO_STATE_READY);
        go->SetUInt32Value(GAMEOBJECT_FLAGS, go->GetGOInfo()->flags);
        go->AddObjectToRemoveList();

        return 0;
    }

    /**
     * Moves this [GameObject] server-side to (x, y, z, o) and returns true,
     * or returns false without moving if the destination lies in a different
     * grid CELL (or the object is not in world).
     *
     * Same-cell only ON PURPOSE: within one cell the grid registration and
     * viewer visibility are untouched, so a bare Relocate is safe - no
     * cell/grid surgery. Callers pair it with [GameObject:Refresh] (3.3.5
     * clients only read GO position from the create block) and fall back to
     * a remove+respawn when this returns false (cell-boundary crossings).
     * Built for the GOCatalog staging ghost: one persistent object gliding
     * with the GM instead of a new spawn every tick.
     *
     * @param float x
     * @param float y
     * @param float z
     * @param float o : orientation (yaw baked into the create block)
     * @return bool moved
     */
    int SetWorldPosition(Eluna* E, GameObject* go)
    {
        float x = E->CHECKVAL<float>(2);
        float y = E->CHECKVAL<float>(3);
        float z = E->CHECKVAL<float>(4);
        float o = E->CHECKVAL<float>(5);

        if (!go->IsInWorld())
        {
            E->Push(false);
            return 1;
        }

        CellPair oldCell = MaNGOS::ComputeCellPair(go->GetPositionX(), go->GetPositionY());
        CellPair newCell = MaNGOS::ComputeCellPair(x, y);
        if (oldCell.x_coord != newCell.x_coord || oldCell.y_coord != newCell.y_coord)
        {
            E->Push(false);
            return 1;
        }

        go->Relocate(x, y, z, o);
        E->Push(true);
        return 1;
    }

    ElunaRegister<GameObject> GameObjectMethods[] =
    {
        // Getters
        { "GetDisplayId", &LuaGameObject::GetDisplayId },
        { "GetGoState", &LuaGameObject::GetGoState },
        { "GetLootState", &LuaGameObject::GetLootState },
        { "GetLootRecipient", &LuaGameObject::GetLootRecipient },
        { "GetLootRecipientGroup", &LuaGameObject::GetLootRecipientGroup },
        { "GetDBTableGUIDLow", &LuaGameObject::GetDBTableGUIDLow },

        // Setters
        { "SetGoState", &LuaGameObject::SetGoState },
        { "SetLootState", &LuaGameObject::SetLootState },
        { "SetRespawnTime", &LuaGameObject::SetRespawnTime },
        { "SetSpawnedByDefault", &LuaGameObject::SetSpawnedByDefault },

        // Boolean
        { "IsTransport", &LuaGameObject::IsTransport },
        { "IsActive", &LuaGameObject::IsActive },
        { "HasQuest", &LuaGameObject::HasQuest },
        { "IsSpawned", &LuaGameObject::IsSpawned },

        // Other
        { "RemoveFromWorld", &LuaGameObject::RemoveFromWorld },
        { "UseDoorOrButton", &LuaGameObject::UseDoorOrButton },
        { "Despawn", &LuaGameObject::Despawn },
        { "Respawn", &LuaGameObject::Respawn },
        { "Refresh", &LuaGameObject::Refresh },
        { "Vanish", &LuaGameObject::Vanish },
        { "SetWorldPosition", &LuaGameObject::SetWorldPosition },
        { "SaveToDB", &LuaGameObject::SaveToDB },

        // Not implemented methods
        { "IsDestructible", METHOD_REG_NONE } // Not implemented
    };
};
#endif
