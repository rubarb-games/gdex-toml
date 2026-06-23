extends Node

enum GemType {
	NONE = 0,
	
	# gems
	RED = 1,
	GREEN = 2,
	BLUE = 3,
	BROWN = 4,

	# resources
	RES_GOLD = 10,
	RES_COPPER = 11,
	RES_COAL = 12,
	MONSTER_PARTS = 13,
	
	# bombs
	BOMB_STANDARD = 20,
	BOMB_DYNAMITE = 21,
	BOMB_LINE = 22,
	BOMB_ESCALATOR = 23,
	BOMB_FIZZLE = 24,
	BOMB_SWAPDIRECTION = 25,
	BOMB_CROSSSWAP = 26,
	BOMB_DELAYEDBLAST = 27,
	BOMB_SQUARESWAP = 28,
	BOMB_COLORSWAP = 29,
	BOMB_OREBLASTER = 30,
	BOMB_MONSTERFAT = 31,
	BOMB_CAUSTICFLASK = 32,
	BOMB_GNAWCHARGE = 33,
	BOMB_DEADMANCHARGE = 34,
	BOMB_SHAFTCHARGE = 35,
	BOMB_GLITTERBOMB = 36,
	BOMB_CHURNCHARGE = 37,
	BOMB_BIGTWENTYFOUR = 38,
	BOMB_BLUEBOX = 39,
	BOMB_STROBOMB = 40,
	BOMB_PROXIMITYCHARGE = 41,
	
	# Last valid bomb is 400
	
	COLLECTABLE_ITEM = 401,
	
	ZERO = 510,
	GRAVEL = 511,
	
	# NOTE: If a value needs to go above 512, remember to also update TileInfo
}

func gem_type_to_str(gt: GemType) -> String:
	var i: int = GemType.values().find(gt)
	if i < 0:
		return "N/A"
	return str(GemType.keys()[i])

func str_to_gem_type(gem_str: String) -> Enums.GemType:
	var gem_type: Variant = GemType.get(gem_str.to_upper())
	if gem_type == null:
		return GemType.NONE
	return gem_type as Enums.GemType

func is_resource(gt: GemType) -> bool:
	return int(gt) >= int(GemType.RES_GOLD) and int(gt) <= int(GemType.RES_COAL)

func is_gem(gt: GemType) -> bool:
	return int(gt) >= GemType.RED and int(gt) <= int(GemType.BROWN)

# TODO: Don't want references to specific bombs here.
# Replace with an array in game state, which contains a list of 
# matchable gems
func is_matchable_gem(gt: GemType) -> bool:
	return is_gem(gt) or gt == GemType.BOMB_PROXIMITYCHARGE

func get_match_gem_type(gt: GemType) -> GemType:
	if gt == GemType.BOMB_PROXIMITYCHARGE:
		return GemType.BOMB_PROXIMITYCHARGE
	return gt

func is_bomb(gt: GemType) -> bool:
	return int(gt) >= GemType.BOMB_STANDARD and int(gt) < GemType.COLLECTABLE_ITEM

func range_gems() -> Array:
	return range(GemType.RED, GemType.BROWN)

func get_random_gem(rng: RandomNumberGenerator) -> Enums.GemType:
	var i: int = rng.randi_range(GemType.RED, GemType.BROWN)
	return i as GemType

enum BombCategory {
	NONE = 0,
	
	#Remember to update the data in game_config.toml
	
	FIVE = 1,
	CROSS = 2,
	FOUR = 3,
	OTHER = 4
}

func str_to_bomb_category(cat_str: String) -> Enums.BombCategory:
	var cat_type: Variant = BombCategory.get(cat_str)
	if cat_type == null:
		return BombCategory.NONE
	return cat_type as Enums.BombCategory

enum PatternType {
	NONE = 0,
	X_SHAPE = 1,
	T_SHAPE = 2,
	L_SHAPE = 3,
	THREE = 4,
	FOUR = 5,
	LONG = 6,
	FOUND_ITEM = 1000,
}

func pattern_type_to_str(pt: PatternType) -> String:
	var i: int = PatternType.values().find(pt)
	if i < 0:
		return "N/A"
	return str(PatternType.keys()[i])

enum MonsterType {
	NONE = 0,
	SPIDER = 1,
	TENTACLE = 2,
	SNAKE = 3,
	BLACK_OOZE = 4,
	GEM_MIMIC = 5,
	ELITE_GEM_MIMIC = 6,
	MUTATED_PLANT = 7,
	
	TEST = 999,
	
	# values 1000 and above are for boss monsters
	BOSS_GEORGE = 1000,
}

func is_boss_monster(mt: MonsterType) -> bool:
	return mt >= 1000

func monster_type_to_str(mt: MonsterType) -> String:
	var i: int = MonsterType.values().find(mt)
	if i < 0:
		return "N/A"
	return str(MonsterType.keys()[i])

func str_to_monster_type(monster: String) -> MonsterType:
	var m_type: Variant = MonsterType.get(monster.to_upper())
	if m_type == null:
		return MonsterType.NONE
	return m_type as MonsterType

enum RootState {
	NONE = 0, 
	IDLE = 1, 
	PAUSE = 2, 
	MINE = 3, 
	TOWN = 4, 
}

func root_state_to_str(rs: RootState) -> String:
	return str(RootState.keys()[rs])

enum Item {
	NONE = 0,
	PICKAXE = 1,
	HORN = 2,
	GUN = 3,
	WHIP = 4,
	NUKE = 5,
	PESTICIDE = 6,
	MONSTERFAT=7,
	CRACKCHARGE=8,
	DRILLCORE=9,
	TUNNELCHARGE=10,
	SHAKENSTIR=11,
	GREENROT=12,
	GREASEDPALM=13,
	ORESLOG=14,
	SCATTERSLOG=15,
	BOOMKIT=16,
	FUSETANGLE=17,
	FUSECUTTER=18,
	BLACKPOWDER=19,
	SHAFTPULLER=20,
	GOLDLEECH=21,
	SHIFTLEVER=22,
	BLUEWAVE=23,
	HEAVYMETAL=24,
	CAUSTICFLASK=25,
	FOREMAN_CANDLE=26,
	
	# Values 1000 and above are considered special
	PROSTHETIC_LEG = 1000,
	GLASS_EYE = 1001,
	BALL_PEEN_HAMMER = 1002,
	BRASS_CALIPERS = 1003,
	
	TREASURE_CHEST = 2000,
	
	# If the range of item values need to go beyond 65.535,
	# remember to code for handling active items in GameState
}

func item_to_str(it: Item) -> String:
	var i: int = Item.values().find(it)
	if i < 0:
		return "N/A"
	return str(Item.keys()[i])

func str_to_item(item_str: String) -> Enums.Item:
	var item_type: Variant = Item.get(item_str.to_upper())
	if item_type == null:
		return Item.NONE
	return item_type as Enums.Item

func is_special_item(item: Item) -> bool:
	return int(item) >= 1000

enum ItemSlotState {
	NONE = 0,
	LOCKED = 1, # Can be unlocked by retrieving a lost item
	UNRESEARCHED = 2, # Can be researched, granting a new item
	EQUIPPED = 3, # Equipped with an item, ready to produce instances
}

func item_slot_state_to_str(iss: ItemSlotState) -> String:
	return str(ItemSlotState.keys()[iss])

enum ItemTier {
	ALL = -1, # Used when querying for items
	UNDEFINED = 0,
	
	TIER_1 = 1,
	TIER_2 = 2,
	TIER_3 = 3,
	
	ALL_BUT_LOST = 999,
	LOST = 1000,
}

func item_tier_to_str(it: ItemTier) -> String:
	var i: int = ItemTier.values().find(it)
	if i < 0:
		return "N/A"
	return str(ItemTier.keys()[i])

func str_to_item_tier(item_str: String) -> Enums.ItemTier:
	var tier: Variant = ItemTier.get(item_str.to_upper())
	if tier == null:
		return ItemTier.UNDEFINED
	return tier as Enums.ItemTier

func val_to_item_tier(val: int) -> Enums.ItemTier:
	var values: Array = Enums.ItemTier.values()
	var i: int = values.find(val)
	if i < 0:
		return Enums.ItemTier.UNDEFINED
	return values[i] as Enums.ItemTier

#enum ItemCompletion {
	#IMMEDIATE = 0,
	#LATER = 1,
#}

enum ItemProcess {
	CONTINUE = 0,
	STOP = 1,
}

enum Ability {
	NONE = 0,
	
	CHAIN_GANG = 1,
	BEGINNERS_LUCK = 2,
	DYING_LIGHT = 3,
	FIVE_ON_THREE = 4,
	GEM_SNIPER = 5,
	GRAVEL_EVERYDAY = 6,
	TWIN_SPARK = 7,
	PAINT_IT_RED = 8,
	PAYDAY = 9,
	GOLD_SNIPER = 10,
	STEADY_HANDS = 11,
	SHORT_FUSE = 12,
	HOARDER = 13,
	SCRAPYIELD = 14,
	LOOSE_ENDS = 15,
	BOMB_CACHE = 16,
	FRONT_LOADED = 17,
	BROWN_DETONATOR = 18,
	DEATH_BLAST = 19,
}

func ability_to_str(ab_t: Ability) -> String:
	var i: int = Ability.values().find(ab_t)
	return str(Ability.keys()[i])

func str_to_ability(ab_str: String) -> Enums.Ability:
	var ab_type: Variant = Ability.get(ab_str.to_upper())
	if ab_type == null:
		return Ability.NONE
	return ab_type as Enums.Ability

enum InputDeviceType { MOUSE_KEYBOARD, GAMEPAD }

func device_type_to_str(dt: InputDeviceType) -> String:
	return str(InputDeviceType.keys()[dt])

enum Direction {
	NONE = 0,
	LEFT = 1,
	RIGHT = 2,
	UP = 3,
	DOWN = 4,
}

func direction_to_str(d: Direction) -> String:
	var i: int = Direction.values().find(d)
	return str(Direction.keys()[i])

enum Lateral {
	NONE = 0,
	LEFT = 1,
	RIGHT = 2,
}

func lateral_to_str(l: Lateral) -> String:
	var i: int = Lateral.values().find(l)
	return str(Lateral.keys()[i])

enum AbilityTriggerResponse {
	NONE = 0,
	DEFAULT = 1,
}

func ability_trigger_to_str(ab_t: AbilityTriggerResponse) -> String:
	var i: int = AbilityTriggerResponse.values().find(ab_t)
	return str(AbilityTriggerResponse.keys()[i])

enum UnlockType {
	NONE = 0,
	ALWAYS = 1,
	UNLOCKED_BY_PROGRESSION = 2,
	LOCKED_IN_DEMO = 3
}

enum InputActionState {
	NONE = 0,
	PRESSED = 1,
	HOLDING = 2,
	RELEASED = 3,
}

func input_action_state_to_str(ias: InputActionState) -> String:
	var i: int = InputActionState.values().find(ias)
	return str(InputActionState.keys()[i])

enum InputCursorSpace {
	SCREEN = 0,
	RELATIVE = 1,
}

func cursor_space_to_str(cs: InputCursorSpace) -> String:
	var i: int = InputCursorSpace.values().find(cs)
	return str(InputCursorSpace.keys()[i])

enum GameType {
	DEMO = 0,
	RELEASE = 1,
	DEFAULT = 2,
}

enum ColorMode {
	NORMAL = 0,
	PROTANOPIA = 1,
	DEUTERANOPIA = 2,
	TRITANOPIA = 3,
	ACHROMATOPSIA = 4
}

enum TileDamageResult {
	TILE_LOCKED = 0,
	TILE_DEAD = 1,
	TILE_HURT = 2
}

# TODO: Reconsider this one. It isn't being used for anything
#enum HitType {
	#IGNORE_HIT = -1,
	#DIRECT_HIT = 0,
	#ADJACENT_HIT = 1,
	#SPECIAL_HIT = 2
#}
#
#func hittype_to_str(it: HitType) -> String:
	#var i: int = HitType.values().find(it)
	#if i < 0:
		#return "N/A"
	#return str(HitType.keys()[i])

enum ChallengeProgress {
	NONE = 0, #Take out of pool - won't be tracked
	IN_PROGRESS = 1,
	UNLOCKED = 2 
}

enum Challenge {
	NONE = -1,
	ALWAYS_UNLOCKED = 0,
	GRAVEL_GRIND_A = 1,
	GRAVEL_GRIND_B = 2,
	DEBUG_EVERY_MOVE = 3,
	ITS_A_START = 4,
	KLONDIKE_BORN = 5,
	BRING_IT_DOWN = 6,
	MY_EYES = 7,
	TOO_DEEP = 8,
	EARLY_LEVEL_10 = 9,

}

func challenge_to_str(it: Challenge) -> String:
	var i: int = Challenge.values().find(it)
	if i < 0:
		return "N/A"
	return str(Challenge.keys()[i])

func str_to_challenge(chg_str: String) -> Enums.Challenge:
	var challenge_type: Variant = Challenge.get(chg_str.to_upper())
	if challenge_type == null:
		return Challenge.NONE
	return challenge_type as Enums.Challenge
	
enum Building {
	NONE = -1,
	MINE = 1,
	LAB = 2,
	WORKSHOP = 3,
	MECHANIC = 4,
	ADVENTURERS_GUILD = 5,
	GAMBLING_SHACK = 6,
	TEMPLATE = 7,
	PAINTER_STUDIO = 8,
	TELLURIUM = 9,
	CHURCH = 10,
	TRAIN_STATION = 11,
	BUTCHER = 12
}

func str_to_building(p_str:String) -> Building:
	var key:Building = Building.get(p_str)
	return key

func building_to_str(b:Building) -> String:
	var key:String = Building.find_key(b)
	return key

enum EnvironmentType {
	NONE = -1,
	WINTER = 1
}

func str_to_environment_type(p_str:String) -> EnvironmentType:
	var key:EnvironmentType = EnvironmentType.get(p_str)
	return key
	
func environment_type_to_str(et:EnvironmentType) -> String:
	var key:String = EnvironmentType.find_key(et)
	return key
