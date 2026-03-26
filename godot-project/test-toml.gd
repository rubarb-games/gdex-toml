extends Node

func p(prefix: String, val: Variant) -> void:
	print("%s : %s" % [prefix, str(val)])

func _ready() -> void:
	test_creator()
	#test_parser()

func test_creator():
	var toml: TomlCreator = TomlCreator.new()
	print("=== TomlCreator ===================\n")

	toml.set_int("my_int", 1704)
	toml.set_int("my_int", 1024) # overwrites the previously set my_int

#	toml.set_int_at(["audio"], "master_volume", 100)
	toml.set_int_at(["a", "b", "c"], "master_volume", 100)

	toml.set_table_at(["ability", "paint_it_red"], {
		abc = 1234,
		hei = "sveis"
	})
#	toml.test(["ability", "gravel_everyday"], {
#		abc = 246,
#		hei = "hade"
#	})

	var ts: String = toml.serialize()
	print("toml created:\n")
	print(ts)
	print("---\n")

#	var tp: TomlParser = TomlParser.new()
#	tp.try_parse(ts)

#	p("paint_it_red", tp.get_table_at(["ability", "paint_it_red"]))

func test_parser():
	var parser = TomlParser.new()
	print("\nTomlParser ===================\n")
	var path: String = "res://sample.toml"
	if load_toml_file(parser, [path]) != OK:
		print("Failed to load sample file from "+path)
		return

	p("orphan_str", parser.get_string("orphan_str"))
	p("items", parser.get_table("items"))
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
