extends Node

func p(prefix: String, val: Variant) -> void:
	print("%s : %s" % [prefix, str(val)])

func _ready() -> void:
	print("\nTomlParser ===================\n")

	var toml: TomlParser = TomlParser.new()
	var path: String = "res://sample.toml"
	if load_toml_file(toml, [path]) != OK:
		print("Failed to load sample file from "+path)
		return

	#p("default_moves", toml.get_int_at(["gameplay", "default_moves"]))
	#p("default_gravel_per_tile", toml.get_int_at(["gameplay", "default_gravel_per_tile"]))
	#
	#p("map_cols", toml.get_int_at(["map", "map_cols"]))
	#p("map_rows", toml.get_int_at(["map", "map_rows"]))
	p("map_tile_size", toml.get_int_at(["map", "map_tile_size"]))
	
	#resource_coal   = toml.get_int_arr_at(["levels", "coal"])
	#resource_copper = toml.get_int_arr_at(["levels", "copper"])
	#resource_gold   = toml.get_int_arr_at(["levels", "gold"])
	#monster_spider  = toml.get_int_arr_at(["levels", "spider"])
	#
	#health_coal   = toml.get_int_at(["levels", "health_coal"])
	#health_copper = toml.get_int_at(["levels", "health_copper"])
	#health_gold   = toml.get_int_at(["levels", "health_gold"])
	#
	#var levelnames: Dictionary = toml.get_table("level_name")
	#for key: String in levelnames:
		#if key == "default":
			#level_names[key] = levelnames[key]
		#else:
			#level_names[int(key)] = levelnames[key]
	#
	#bomb_handlers = toml.get_string_arr_at(["bombs", "handlers"])
	#
	#volume_master = ObservableInt.new(toml.get_int_at(["audio", "volume_master"]))
	#volume_music = ObservableInt.new(toml.get_int_at(["audio", "volume_music"]))
	#volume_sfx = ObservableInt.new(toml.get_int_at(["audio", "volume_sfx"]))
	#
	#display_fullscreen = ObservableBool.new(toml.get_bool_at(["display", "fullscreen"]))

	
	print("\nTomlCreator ===================\n")

	var tw: TomlCreator = TomlCreator.new()
	tw.set_int("my_int", 1704)
	tw.set_int("my_int", 1024) # overwrites the previously set my_int
	tw.set_color("color", Color.BROWN)
	tw.set_vector2("vec2", Vector2(0.5, 0.85))
	tw.set_vector2i("vec2i", Vector2i(5, 85))
	tw.set_variant("variant", true)
	tw.set_array("arr", ["hey", 1.0, 2, false, Vector2(0.25, 0.9)])
	tw.set_int_to_section("my-section", "some_int", 1704)
	tw.set_int_to_section("my-section", "some_int", 1802)
	tw.set_dictionary("dict1", {
		"hello": "world",
		"nested": { "whatever": true },
		"arr_dict": [{ "key": true }, { "key": false }]
	})
	tw.set_int_to_section("my-section", "other_int", 1802)
	tw.set_bool_to_section("my-section", "doodad", false)
	tw.set_color("color_x", Color.CORNFLOWER_BLUE)
	tw.format_array("arr", TomlCreator.ArrayFormat.AF_ONELINE)
	tw.format_table("dict1", TomlCreator.TF_MULTILINE, 0, 0, 0, TomlCreator.IC_SPACE)
	print(tw.serialize())


static func load_toml_file(p_parser: TomlParser, p_paths: Array) -> int:
	for path in p_paths:
		var file = FileAccess.open(path, FileAccess.READ)
		if file == null:
			continue
		var content = file.get_as_text()
		if p_parser.try_parse(content):
			#print("helpers.load_toml_file :: loaded and parsed toml file: "+path)
			return OK
	return ERR_CANT_OPEN
