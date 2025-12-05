extends Node

func p(prefix: String, val: Variant) -> void:
	print("%s : %s" % [prefix, str(val)])

#var parser: TomlParser
#var creator: TomlCreator

func _ready() -> void:
	print("\nTomlParser ===================\n")
	
	
	#print("\nTomlParser ===================\n")

	var parser = TomlParser.new()
	var path: String = "res://sample.toml"
	if load_toml_file(parser, [path]) != OK:
		print("Failed to load sample file from "+path)
		return

	p("orphan_str", parser.get_string("orphan_str"))
	#p("default_moves", parser.get_int_at(["gameplay", "default_moves"]))
	#p("default_gravel_per_tile", parser.get_int_at(["gameplay", "default_gravel_per_tile"]))
	#
	#p("map_cols", parser.get_int_at(["map", "map_cols"]))
	#p("map_rows", parser.get_int_at(["map", "map_rows"]))
	#p("map_tile_size", parser.get_float_at(["map", "map_tile_size"]))
	
	#resource_coal   = parser.get_int_arr_at(["levels", "coal"])
	#resource_copper = parser.get_int_arr_at(["levels", "copper"])
	#resource_gold   = parser.get_int_arr_at(["levels", "gold"])
	#monster_spider  = parser.get_int_arr_at(["levels", "spider"])
	#
	#health_coal   = parser.get_int_at(["levels", "health_coal"])
	#health_copper = parser.get_int_at(["levels", "health_copper"])
	#health_gold   = parser.get_int_at(["levels", "health_gold"])
	#
	#var levelnames: Dictionary = parser.get_table("level_name")
	#for key: String in levelnames:
		#if key == "default":
			#level_names[key] = levelnames[key]
		#else:
			#level_names[int(key)] = levelnames[key]
	#
	#bomb_handlers = parser.get_string_arr_at(["bombs", "handlers"])
	#
	#volume_master = ObservableInt.new(parser.get_int_at(["audio", "volume_master"]))
	#volume_music = ObservableInt.new(parser.get_int_at(["audio", "volume_music"]))
	#volume_sfx = ObservableInt.new(parser.get_int_at(["audio", "volume_sfx"]))
	#
	#display_fullscreen = ObservableBool.new(parser.get_bool_at(["display", "fullscreen"]))
	#parser.free()
	
	#print("\nTomlCreator ===================\n")

	#creator = TomlCreator.new()
	#creator.set_int("my_int", 1704)
	#creator.set_int("my_int", 1024) # overwrites the previously set my_int
	#creator.set_color("color", Color.BROWN)
	#creator.set_vector2("vec2", Vector2(0.5, 0.85))
	#creator.set_vector2i("vec2i", Vector2i(5, 85))
	#creator.set_variant("variant", true)
	#creator.set_array("arr", ["hey", 1.0, 2, false, Vector2(0.25, 0.9)])
	#creator.set_int_to_section("my-section", "some_int", 1704)
	#creator.set_int_to_section("my-section", "some_int", 1802)
	#creator.set_dictionary("dict1", {
		#"hello": "world",
		#"nested": { "whatever": true },
		#"arr_dict": [{ "key": true }, { "key": false }]
	#})
	#creator.set_int_to_section("my-section", "other_int", 1802)
	#creator.set_bool_to_section("my-section", "doodad", false)
	#creator.set_color("color_x", Color.CORNFLOWER_BLUE)
	#creator.format_array("arr", TomlCreator.ArrayFormat.AF_ONELINE)
	#creator.format_table("dict1", TomlCreator.TF_MULTILINE, 0, 0, 0, TomlCreator.IC_SPACE)
	#print(creator.serialize())
	#creator.free()

#func _exit_tree() -> void:
	#parser = null
	#creator = null
	#parser.free()
	#creator.free()

#func load_toml(p_parser: TomlParser, p_path: String, p_dev: bool) -> int:
	#if p_dev:
		#var res: Resource = ResourceLoader.load(p_path)

static func load_toml_file(p_parser: TomlParser, p_paths: Array) -> int:
	for path in p_paths:
		#ResourceLoader.load()
		var file = FileAccess.open(path, FileAccess.READ)
		if file == null:
			continue
		var content = file.get_as_text()
		if p_parser.try_parse(content):
			#print("helpers.load_toml_file :: loaded and parsed toml file: "+path)
			return OK
	return ERR_CANT_OPEN
