# Splitgate Forge Importer

This is a tool to help people load their forge maps into unreal engine with a basic view, mainly for moving blockout concepts around.

## Support

Currently this plugin only supports the following:

* Unreal Engine 4.26.*
* Unreal Engine 4.27.*
* Unreal Engine 5.0 -> 5.6

After the base has been finished on 4.26 more versions above such as unreal engine 5 will be added.

## Usage
Get a Forge map you wish to use from either [SplitgateForgeBackup](https://github.com/Splitgate/SplitgateForgeBackup) or from [Capturing your own](#capturing-your-own-forge-map)

#### **Step One:** 
From the Unreal Engine viewport select the ```Modes``` drop-down from the top toolbar list and select Forge Importer, it will list a few options,

```Import Path - Locate the .JSON forge file```\
```Import Only Static Meshes - this will make the importer only spawn static meshes without any proprietery C++ classes from this plugin```

#### **Step Two:**
Press Load, now you should have the Forge map loaded! 

## Capturing your own forge map

If you wish to capture your own Forge map data without relying on [SplitgateForgeBackup](https://github.com/Splitgate/SplitgateForgeBackup).

> [!WARNING]
> **Be Careful with this and do not try and fiddle any requests asides one in specific else it may cause problems**

#### **Step One:** 
Install Fiddler or any other http proxy tool,
You can get Fiddler Classic or Fiddler Everywhere from [here](https://docs.telerik.com/fiddler/configure-fiddler/tasks/installfiddler) (I reccomend classic).

#### **Step Two:** 
Make sure proxy is loaded and capturing requests from the game,

Ensure if you are using Fiddler Classic, you have *Stream* and *Decode* toggled at the top bar, and in Tools>Options>HTTPS and enable the following:

```HTTPS Connects```\
```Decrypt HTTPS traffic```\
```Ignore server certificate errors```

#### **Step Three:** 
In game go to Forge and download the map code you want (ideally your own) and await for any requests similar to ```https://splitgate.accelbyte.io/ugc/v1/public/namespaces/splitgate/maps/sharecodes/XXX-XXX-XXX``` the data this request responds with contains the forge data that you will use.

## Current Problems

Currently this tool is missing a few things when importing the maps that being the following list. (Games actual path is /Game not /ForgeImporter)
```
/ForgeImporter/Blueprints/GameModes/Forge/Interactables/GameModes/CTF/InteractableFlagCaptureZone_Alpha_BP
/ForgeImporter/Blueprints/GameModes/Forge/Interactables/GameModes/CTF/InteractableFlagCaptureZone_Bravo_BP
/ForgeImporter/Blueprints/GameModes/Forge/Interactables/GameModes/CTF/InteractableNeutralFlagCaptureZone_BP
/ForgeImporter/Blueprints/GameModes/Forge/Interactables/GameModes/Domination/InteractableDominationHill_Alpha_BP
/ForgeImporter/Blueprints/GameModes/Forge/Interactables/GameModes/Domination/InteractableDominationHill_Bravo_BP
/ForgeImporter/Blueprints/GameModes/Forge/Interactables/GameModes/Domination/InteractableDominationHill_Charlie_BP
/ForgeImporter/Blueprints/GameModes/Forge/Interactables/GameModes/KOTH/InteractableKOTHHill_1_BP
/ForgeImporter/Blueprints/GameModes/Forge/Interactables/GameModes/KOTH/InteractableKOTHHill_2_BP
/ForgeImporter/Blueprints/GameModes/Forge/Interactables/GameModes/KOTH/InteractableKOTHHill_3_BP
/ForgeImporter/Blueprints/GameModes/Forge/Interactables/GameModes/KOTH/InteractableKOTHHill_4_BP
/ForgeImporter/Blueprints/GameModes/Forge/Interactables/GameModes/KOTH/InteractableKOTHHill_5_BP
/ForgeImporter/Blueprints/GameModes/Forge/Interactables/GameModes/KOTH/InteractableKOTHHill_6_BP
/ForgeImporter/Blueprints/GameModes/Forge/Interactables/Volumes/InteractableBoxOutOfBoundsVolume_BP
/ForgeImporter/Blueprints/GameModes/Forge/Interactables/Volumes/InteractableBoxKillZVolume_BP
/ForgeImporter/Blueprints/GameModes/Forge/Interactables/Volumes/InteractableBoxBlockingVolume_BP
/ForgeImporter/Blueprints/GameModes/Forge/Interactables/Portals/InteractablePortalWallFrame_2x4x4_BP
/ForgeImporter/Blueprints/GameModes/Forge/Interactables/Portals/InteractablePortalWallFrame_2x4x8_BP
/ForgeImporter/Blueprints/GameModes/Forge/Interactables/Portals/InteractablePortalWallFrame_2x8x4_BP
/ForgeImporter/Blueprints/GameModes/Forge/Interactables/Portals/InteractablePortalWallFrame_Angled_4x4x4_BP
/ForgeImporter/Blueprints/GameModes/Forge/Interactables/Portals/InteractablePortalWallFrame_Angled_8x4x8_BP
/ForgeImporter/Blueprints/GameModes/Forge/Interactables/Portals/InteractablePortalWallFrame_Bevel_4x4x4_BP
/ForgeImporter/Blueprints/GameModes/Forge/Interactables/Portals/InteractablePortalWallFrame_Floor_4x4x4_BP
/ForgeImporter/Blueprints/GameModes/Forge/Interactables/Portals/InteractablePortalWallFrame_Floor_Round_8x4x8_BP
/ForgeImporter/Blueprints/GameModes/Forge/Interactables/Portals/InteractablePortalWallFrame_Wedge_4x4x4_BP
/ForgeImporter/Blueprints/GameModes/Forge/Interactables/Portals/InteractablePortalWallFrame_Wedge_4x4x8_BP
/ForgeImporter/Blueprints/GameModes/Forge/Interactables/GameModes/Lockdown/InteractableLockdownHill_0_A_BP
/ForgeImporter/Blueprints/GameModes/Forge/Interactables/GameModes/Lockdown/InteractableLockdownHill_0_B_BP
/ForgeImporter/Blueprints/GameModes/Forge/Interactables/GameModes/Lockdown/InteractableLockdownHill_0_C_BP
/ForgeImporter/Blueprints/GameModes/Forge/Interactables/GameModes/Lockdown/InteractableLockdownHill_1_A_BP
/ForgeImporter/Blueprints/GameModes/Forge/Interactables/GameModes/Lockdown/InteractableLockdownHill_1_B_BP
/ForgeImporter/Blueprints/GameModes/Forge/Interactables/GameModes/Lockdown/InteractableLockdownHill_1_C_BP
/ForgeImporter/Blueprints/GameModes/Forge/Interactables/GameModes/Lockdown/InteractableLockdownHill_2_A_BP
/ForgeImporter/Blueprints/GameModes/Forge/Interactables/GameModes/Lockdown/InteractableLockdownHill_2_B_BP
/ForgeImporter/Blueprints/GameModes/Forge/Interactables/GameModes/Lockdown/InteractableLockdownHill_2_C_BP
/ForgeImporter/Blueprints/GameModes/Forge/Interactables/GameModes/Lockdown/InteractableLockdownHill_3_A_BP
/ForgeImporter/Blueprints/GameModes/Forge/Interactables/GameModes/Lockdown/InteractableLockdownHill_3_B_BP
/ForgeImporter/Blueprints/GameModes/Forge/Interactables/GameModes/Lockdown/InteractableLockdownHill_3_C_BP
```
