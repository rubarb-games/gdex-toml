class_name GameConfigIO extends RefCounted

static func read_config(file_path: String) -> TomlParser:
	var parser: TomlParser = TomlParser.new()
	parser.logging(true)
	if Utils.load_toml_file(parser, [file_path]) != OK:
		print("game_config_io :: Could not load config %s" % [file_path])
		return null
	print("game_config_io :: Loaded config %s" % [file_path])
	return parser

static func load_config(cfg: GameConfig) -> void:
	var toml_cfg: TomlParser = read_config("res://data/game_config.toml")
	var toml_ab: TomlParser = read_config("res://data/abilities.toml")
	var toml_items: TomlParser = read_config("res://data/items.toml")
	var toml_monsters: TomlParser = read_config("res://data/monsters.toml")
	var toml_bombs: TomlParser = read_config("res://data/bombs.toml")
	
	
	
	# GAME CONFIG
	
	#Simon: this should get replaced in time with something nicer :)
	#game_type handles top level flags, like if the game is in a demo state
	var is_demo = toml_cfg.get_bool_at(["game_type","is_demo"])
	if is_demo:
		cfg.game_type = Enums.GameType.DEMO #Set game to demo state
	else:
		cfg.game_type = Enums.GameType.RELEASE #Set game to release state
	
	var gameplay: Dictionary = toml_cfg.get_table("gameplay")
	cfg.default_moves = gameplay.default_moves
	cfg.default_gravel_per_tile = gameplay.default_gravel_per_tile
	for allowed in gameplay.allowed_swaps:
		cfg.allowed_swaps.append(Enums.str_to_gem_type(allowed))
	cfg.default_coal = gameplay.default_coal
	cfg.default_copper = gameplay.default_copper
	cfg.default_gold = gameplay.default_gold
	cfg.default_gravel = gameplay.default_gravel
	cfg.mine_depth = gameplay.mine_depth
	cfg.total_days = gameplay.total_days
	
	var towns: Dictionary = toml_cfg.get_table("town")
	print(towns)
	for town_id: String in towns:
		var town_cfg: TownConfig = TownConfig.new()
		# load here
		cfg.towns[town_id] = town_cfg
	
	var map: Dictionary = toml_cfg.get_table("map")
	print(map)
	cfg.map_cols = map.map_cols
	cfg.map_rows = map.map_rows
	cfg.map_tile_size = map.map_tile_size
	
	var levels: Dictionary = toml_cfg.get_table("levels")
	print(levels)
	cfg.resource_coal   = toml_cfg.get_int_arr_at(["levels", "resource", "coal"])
	cfg.resource_copper = toml_cfg.get_int_arr_at(["levels", "resource", "copper"])
	cfg.resource_gold   = toml_cfg.get_int_arr_at(["levels", "resource", "gold"])
	
	cfg.health_coal = levels.resource.health_coal
	cfg.health_copper = levels.resource.health_copper
	cfg.health_gold = levels.resource.health_gold
	
	for key: String in levels.name:
		if key == "default":
			cfg.level_names[key] = levels.name[key]
		else:
			cfg.level_names[int(key)] = levels.name[key]
	
	var item_levels: Array[int]
	for key: String in levels.lost_item:
		item_levels.append(int(key))
	item_levels.sort()
	for level: int in item_levels:
		cfg.lost_items_tbl[level] = Enums.str_to_item(levels.lost_item[str(level)])
	
	cfg.bossfight_at_level = levels.bossfight.bossfight_at_level
	
	cfg.bossfight_sequence.clear()
	for boss_id: String in levels.bossfight.bossfight_sequence:
		var monster_type: Enums.MonsterType = Enums.str_to_monster_type(boss_id)
		cfg.bossfight_sequence.append(monster_type)
	
	var audio: Dictionary = toml_cfg.get_table("audio")
	print(audio)
	cfg.volume_master = ObservableInt.new(audio.volume_master)
	cfg.volume_music = ObservableInt.new(audio.volume_music)
	cfg.volume_sfx = ObservableInt.new(audio.volume_sfx)
	
	cfg.show_tutorial_popups = ObservableBool.new(toml_cfg.get_bool_at(["gameplay", "show_tutorial_popups"]))
	cfg.display_fullscreen = ObservableBool.new(toml_cfg.get_bool_at(["display", "fullscreen"]))
	
	# Items
	
	var items: Dictionary = toml_items.get_table("items")
	print(items)
	cfg.max_items_equipped = items.max_items_equipped
	cfg.health_collectable_item = items.health_collectable_item

	# Items slots

	var item_slots: Dictionary = toml_items.get_table("item_slot")
	print(item_slots)
	for slot_id: String in item_slots:
		var item_slot: ItemSlotDefinition = ItemSlotDefinition.new()
		var src: Dictionary = item_slots[slot_id]
		
		item_slot.tier = Enums.val_to_item_tier(int(src.tier))
		item_slot.state = Enums.ItemSlotState.get(src.default_state.to_upper())
		item_slot.price_tbl = PriceTbl.dec_price_list(src.price)
		
		item_slot.locked_by = Enums.Item.NONE
		if src.has("locked_by"):
			item_slot.locked_by = Enums.str_to_item(src.locked_by)
		
		item_slot.equipped = Enums.Item.NONE
		if src.has("equipped"):
			item_slot.equipped = Enums.str_to_item(src.equipped)
		
		cfg.items_slots[int(slot_id)] = item_slot
	
	# Item Fx
	
	var item_fx: Dictionary = toml_items.get_table("item_fx")
	print(item_fx)
	for fx_id: String in item_fx:
		var scene_filename: String = item_fx[fx_id].scene
		cfg.itemfx_scenepath_tbl[fx_id] = "res://prefabs/item_fx/%s.tscn" % scene_filename
	
	# Item definitions
	
	print("items ____")
	var item_defs: Dictionary = toml_items.get_table("item")
	print(item_defs)
	for item_id: String in item_defs:
		continue
		var def: Dictionary = item_defs[item_id]
		var item_type: Enums.Item = Enums.str_to_item(item_id)
		var item_def: ItemDefinition = ItemDefinition.new()

		item_def.tier = Enums.val_to_item_tier(int(def.tier))
		item_def.item_type = item_type
		item_def.name_key = "ITEM_%s_NAME" % item_id.to_upper()
		item_def.description_key = "ITEM_%s_DESC" % item_id.to_upper()
		
		_load_base_def(def, item_def)
		
		var atlas_pos: Vector2i = Vector2i(7, 7)
		if def.has("atlas_pos"):
			atlas_pos.x = def.atlas_pos[0]
			atlas_pos.y = def.atlas_pos[1]
		item_def.atlas_pos = atlas_pos
		
		if def.has("script"):
			item_def.script_path = "res://entities/items/%s.gd" % def.script
		elif def.has("scene"):
			item_def.scene_path = "res://entities/items/%s.tscn" % def.scene
		
		if def.has("item_fx"):
			item_def.item_fx = def.item_fx
		else:
			item_def.item_fx = "none"
		
		if def.has("unlock_type"):
			item_def.unlock_type = def.unlock_type
		else:
			item_def.unlock_type = Enums.UnlockType.ALWAYS
		
		if def.has("challenge"):
			item_def.associated_challenge = Enums.str_to_challenge(def["challenge"])
		
		cfg.items[item_type] = item_def

	# Bombs
	
	print("bombs ____")
	for bomb_id: String in toml_bombs.get_table_keys("bomb"):
		
		var bomb_type: Enums.GemType = Enums.str_to_gem_type("BOMB_" + bomb_id)
		if bomb_type == Enums.GemType.NONE:
			print("config", "Cannot find a corresponding enum for bomb '%s'. "+\
			"Check that table [bomb.%s] of game_config.toml is correctly spelled." % [bomb_id, bomb_id])
			continue
		
		var def: Dictionary = toml_bombs.get_table_at(["bomb", bomb_id])
		print(def)
		var bomb_def: BombDefinition = BombDefinition.new()
		bomb_def.name_key = "BOMB_%s_NAME" % bomb_id.to_upper()
		bomb_def.description_key = "BOMB_%s_DESC" % bomb_id.to_upper()
		_load_base_def(def, bomb_def)
		
		
	print("000000000")
	
	var bomb_defs: Dictionary = toml_bombs.get_table("bomb")
	print(bomb_defs)
	for bomb_id: String in bomb_defs:
		continue
		var bomb_type: Enums.GemType = Enums.str_to_gem_type("BOMB_" + bomb_id)
		if bomb_type == Enums.GemType.NONE:
			print("config", "Cannot find a corresponding enum for bomb '%s'. "+\
			"Check that table [bomb.%s] of game_config.toml is correctly spelled." % [bomb_id, bomb_id])
			continue

		var def: Dictionary = bomb_defs[bomb_id]
		var bomb_def: BombDefinition = BombDefinition.new()
		bomb_def.name_key = "BOMB_%s_NAME" % bomb_id.to_upper()
		bomb_def.description_key = "BOMB_%s_DESC" % bomb_id.to_upper()
		_load_base_def(def, bomb_def)
		
		bomb_def.category = Enums.str_to_bomb_category(def.category)
		bomb_def.bomb_type = bomb_type
		bomb_def.script_path = "res://entities/bombs/bomb_%s.gd" % def.script
		
		var atlas_pos: Vector2i = Vector2i(7, 7)
		if def.has("atlas_pos"):
			atlas_pos.x = def.atlas_pos[0]
			atlas_pos.y = def.atlas_pos[1]
		bomb_def.atlas_pos = atlas_pos
		
		cfg.bombs[bomb_type] = bomb_def
		
		if def.has("settings"):
			cfg.bomb_settings[bomb_type] = def["settings"].duplicate()
			
		if def.has("unlock_type"):
			bomb_def.unlock_type = def.unlock_type
		else:
			bomb_def.unlock_type = Enums.UnlockType.ALWAYS

		if def.has("challenge"):
			bomb_def.associated_challenge = Enums.str_to_challenge(def["challenge"])
	#print("1")

	# Abilities
	#var ab_conf: Dictionary = toml_ab.get_table("config")
	
	print("abilities ____")
	var abilities: Dictionary = toml_ab.get_table("ability")
	print(abilities)
	for ab_id: String in abilities:
		var ab_type: Enums.Ability = Enums.str_to_ability(ab_id)
		if ab_type == Enums.Ability.NONE:
			print("config", "Cannot find a corresponding enum for ability '%s'. "+\
			"Check that table [ability.%s] of abilities.toml is correctly spelled." % [ab_id, ab_id])
			continue
		
		var def: Dictionary = abilities[ab_id]
		var ab_def: AbilityDefinition = AbilityDefinition.new()
		ab_def.name_key = "ABILITY_%s_NAME" % ab_id.to_upper()
		ab_def.description_key = "ABILITY_%s_DESC" % ab_id.to_upper()
		
		_load_base_def(def, ab_def)
		
		ab_def.type = ab_type
		ab_def.script_path = "res://entities/abilities/ab_%s.gd" % def.script
		
		if def.has("unlock_type"):
			ab_def.unlock_type = def.unlock_type
		else:
			ab_def.unlock_type = Enums.UnlockType.ALWAYS
		
		if def.has("challenge"):
			ab_def.associated_challenge = Enums.str_to_challenge(def["challenge"])
		
		cfg.abilities[ab_type] = ab_def
	print("2")

	# Monsters
	var m_conf: Dictionary = toml_monsters.get_table("config")
	cfg.default_boss_intro = "res://%s.tscn" % m_conf.default_boss_intro
	cfg.default_boss_outro = "res://%s.tscn" % m_conf.default_boss_outro
	
	var monsters: Dictionary = toml_monsters.get_table("monster")
	for monster_id: String in monsters:
		var monster_type: Enums.MonsterType = Enums.str_to_monster_type(monster_id)
		if monster_type == Enums.MonsterType.NONE:
			print("config", "Cannot find MonsterType enum for %s" % monster_id)
			continue
		
		var def: Dictionary = monsters[monster_id]
		var m_def: MonsterDefinition = MonsterDefinition.new()
		m_def.name_key = "MONSTER_%s_NAME" % monster_id.to_upper()
		m_def.description_key = "MONSTER_%s_DESC" % monster_id.to_upper()
		
		_load_base_def(def, m_def)
		
		m_def.type = monster_type
		m_def.health = def.health
		m_def.scene_path = "res://entities/monsters/%s.tscn" % def.scene
		
		if def.has("scene_intro"):
			m_def.scene_intro = "res://entities/monsters/%s.tscn" % def.scene_intro
		if def.has("scene_outro"):
			m_def.scene_outro = "res://entities/monsters/%s.tscn" % def.scene_outro
		if def.has("scene_healthbar"):
			m_def.scene_healthbar = "res://entities/monsters/%s.tscn" % def.scene_healthbar
	
		cfg.monsters[monster_type] = m_def
	
	#Create spawn infos for monsters
	for monster_type: Enums.MonsterType in cfg.monsters:
		var monster_str: String = Enums.monster_type_to_str(monster_type).to_lower()
		var spawn_data: Dictionary = toml_cfg.get_table_at(["levels", "monster", monster_str])
		if spawn_data.is_empty():
			print("config", "Missing spawn data for monster %s. Add it to game_config.toml" % monster_str)
			continue
		
		var count_tbl: Dictionary[int, int] # key: level, value: count
		var properties_tbl: Dictionary[int, Array] # key: level, value: array of strings
		var missing_spawn_count: bool = true
		
		if spawn_data.has("levels"):
			missing_spawn_count = false
			for level_i: int in range(0, spawn_data.levels.size()):
				count_tbl[level_i] = spawn_data.levels[level_i]
		
		else:
			for level_i: int in range(0, cfg.mine_depth):
				if not spawn_data.has("level_%d" % level_i):
					continue
				
				missing_spawn_count = false
				var val: Variant = spawn_data["level_%d" % level_i]
				match typeof(val):
					TYPE_INT:
						count_tbl[level_i] = int(val)
					TYPE_ARRAY:
						count_tbl[level_i] = (val as Array).size()
						properties_tbl[level_i] = (val as Array).duplicate()
		
		if missing_spawn_count:
			print("config", "Cannot find spawn data for %s. Add either a levels array, or level_n per level" % monster_str)
		
		for level: int in range(0, cfg.mine_depth):
			# Add an array to the spawn table, even if it will never be populated.
			# This reduces the amount of error checking other parts of the code
			# will have to do.
			if not cfg.monster_spawn_tbl.has(level):
				var arr: Array[MonsterSpawnInfo] = []
				cfg.monster_spawn_tbl[level] = arr
			
			var count: int = 0
			if count_tbl.has(level):
				count = count_tbl[level]
			
			if count == 0:
				# No monsters to spawn at this depth, no need to add an
				# entry to the spawn table
				continue
			
			var properties: Array[String]
			if properties_tbl.has(level):
				for prop: String in properties_tbl[level]:
					properties.append(prop)
			
			var spawn_info: MonsterSpawnInfo = MonsterSpawnInfo.new()
			spawn_info.monster_type = monster_type
			spawn_info.count = count
			spawn_info.priority = int(spawn_data.spawn_priority)
			spawn_info.properties = properties
			
			cfg.monster_spawn_tbl[level].append(spawn_info)

static func _get_icon_color(p_dict: Dictionary) -> Texture2D:
	if p_dict.has("icon_color"):
		return load("res://art/textures/icons/%s.png" % p_dict.icon_color) as Texture2D
	return load("res://art/textures/icons/icon_jack.png") as Texture2D

static func _get_icon_silhouette(p_dict: Dictionary) -> Variant:
	if p_dict.has("icon_silhouette"):
		return load("res://art/textures/icons/%s.png" % p_dict.icon_silhouette) as Texture2D
	return false
	
static func _get_icon_closeup(p_dict: Dictionary) -> Variant:
	if p_dict.has("icon_closeup"):
		return load("res://art/textures/icons/%s.png" % p_dict.icon_closeup) as Texture2D
	return false

static func _load_base_def(src: Dictionary, def: BaseDefinition) -> void:
	def.icon_color = _get_icon_color(src)
	var icon_silhouette = _get_icon_silhouette(src)
	if icon_silhouette:
		def.icon_silhouette = icon_silhouette as Texture2D
		
	var icon_closeup = _get_icon_closeup(src)
	if icon_closeup:
		def.icon_closeup = icon_closeup as Texture2D
	
	if src.has("price"):
		def.price_tbl = PriceTbl.dec_price_list(src.price)
