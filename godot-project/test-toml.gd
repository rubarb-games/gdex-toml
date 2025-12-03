extends Node

func _ready() -> void:
	print("\nTomlParser ===================\n")

	var toml_parser: TomlParser = TomlParser.new()
	var path: String = "res://sample.toml"
	if load_toml_file(toml_parser, [path]) != OK:
		print("Failed to load sample file from "+path)
		return

	print(toml_parser.get_string("sample_string"))
	print(toml_parser.get_string_or("sample_stringx", "blæ"))
	print(toml_parser.get_string_at(["table", "my_str"]))
	print(toml_parser.get_string_arr_at(["table", "my_strings"]))
	
	print(toml_parser.get_color("col"))
	
	print(toml_parser.get_table("table"))
	print(toml_parser.get_table_at(["my_table", "abc"]))
	print(toml_parser.get_int_arr_at(["game_config", "board_size"]))
	print(toml_parser.get_vec2i_at(["game_config", "board_size"]))

	#print(toml_parser.get_int("sample_int"))
	#print(toml_parser.get_int_or("sample_intx", -1))
	#var arr_ints: Array[int] = toml_parser.get_array_int("int_arr")
	#print(arr_ints)
	#var arr_floats: Array[float] = toml_parser.get_array_float("float_arr")
	#print(arr_floats)
	#var arr_strings: Array[String] = toml_parser.get_array_string("string_arr")
	#print(arr_strings)
	#
	#var truths: Dictionary = toml_parser.get_table("my_table")
	#print(truths)
	#
	#var vec2s: Array[Vector2i] = toml_parser.get_array_vector2i(["my_table", "vectors"])
	#print(vec2s)
	
	#var table: Dictionary = toml_parser.get_table("opponent")

	# get game_config section
	#var game_config: Dictionary = toml_parser.get_section("game_config")
	##var board_size: Array = toml_parser.get_arr("game_config.board_size")
	#var board_size: Array = game_config["board_size"] as Array
	#var start_cards: int = game_config["start_cards"] as int
	#var start_cash: int = game_config["start_cash"] as int
#
	#var player_colors: Array[Color]
	#for color in game_config["player_colors"]:
		#player_colors.append(Color(color as String))
#
	#print("GameConfig keys: "+str(game_config.size()))
	#print("Board size: "+str(board_size))
	#print("Start cash: "+str(start_cash))
	#print("Start cards: "+str(start_cards))
	#print("Player colors: "+str(player_colors))
	
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
