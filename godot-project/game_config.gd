class_name GameConfig extends RefCounted

#var cur_savegame: String
var cur_language: String

#Simon: game type defines what state the game is. 0 = Demo, 1 = Release
#This should be replaced down the line
var game_type:Enums.GameType = Enums.GameType.DEFAULT

var default_moves: int
var default_gravel_per_tile: int
var total_days: int

var resource_coal: Array[int]
var resource_copper: Array[int]
var resource_gold: Array[int]
var default_coal: int
var default_copper: int
var default_gold: int
var default_gravel: int
var mine_depth: int

var towns: Dictionary[String, TownConfig]
var level_names: Dictionary
var lost_items_tbl: Dictionary

var map_cols: int
var map_rows: int
var map_tile_size: float

var health_coal: int
var health_copper: int
var health_gold: int

var allowed_swaps: Array[Enums.GemType]

var bomb_settings: Dictionary[Enums.GemType, Dictionary]
var bombs: Dictionary[Enums.GemType, BombDefinition]

var volume_master: ObservableInt
var volume_sfx: ObservableInt
var volume_music: ObservableInt

var display_fullscreen: ObservableBool
var show_tutorial_popups: ObservableBool

var default_matching_vfx:String = "match"

var items_slots: Dictionary[int, ItemSlotDefinition]
var items: Dictionary[Enums.Item, ItemDefinition]
var health_collectable_item: int
var max_items_equipped: int
var itemfx_scenepath_tbl: Dictionary[String, String]

var abilities: Dictionary[Enums.Ability, AbilityDefinition]

var monster_spawn_tbl: Dictionary # key: level index, value: array of SpawnInfo
var monsters: Dictionary[Enums.MonsterType, MonsterDefinition]
var bossfight_at_level: int
var bossfight_sequence: Array[Enums.MonsterType]
var default_boss_intro: String
var default_boss_outro: String

var arachnophobia_filter: bool
var enable_camera_shake: bool
var enable_screen_flash: bool
var gem_contrast:float = 0.0
var colorblind_mode:Enums.ColorMode = Enums.ColorMode.NORMAL
var font_size:int = 0
