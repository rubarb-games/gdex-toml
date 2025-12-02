extends Node

func _ready() -> void:
	var toml_parser: TomlParser = TomlParser.new()
	var path: String = "res://sample.toml"
	if load_toml_file(toml_parser, [path]) != OK:
		print("Failed to load sample file from "+path)
		return

	# get game_config section
	var game_config: Dictionary = toml_parser.get_section("game_config")
	#var board_size: Array = toml_parser.get_arr("game_config.board_size")
	var board_size: Array = game_config["board_size"] as Array
	var start_cards: int = game_config["start_cards"] as int
	var start_cash: int = game_config["start_cash"] as int

	var player_colors: Array[Color]
	for color in game_config["player_colors"]:
		player_colors.append(Color(color as String))

	print("GameConfig keys: "+str(game_config.size()))
	print("Board size: "+str(board_size))
	print("Start cash: "+str(start_cash))
	print("Start cards: "+str(start_cards))
	print("Player colors: "+str(player_colors))
	
	print("\nTomlWriter ===================\n")
	#var toml_str: String = TomlParser.format("something", 1704)
	#print(toml_str)
	#var tbl: Dictionary = {
		#"int": 123,
		#"float": 1.3,
		#"bool": true,
		#"string": "hello!",
		#"arr": [1, 2.4, "something", false],
	#}
	#var tbl_str: String = TomlParser.format_table("table", tbl)
	#print(tbl_str)
	
	var tw: TomlWriter = TomlWriter.new()
	tw.add_int("my_int", 1704)
	tw.add_int("my_int", 1024)
	tw.add_color("color", Color.BROWN)
	tw.add_vector2("vec2", Vector2(0.5, 0.85))
	tw.add_vector2i("vec2i", Vector2i(5, 85))
	tw.add_variant("variant", true)
	tw.add_array("arr", ["hei", 1.0, 2, false, Vector2(0.25, 0.9)])
	tw.add_int_to_section("my-section", "some_int", 1704)
	tw.add_dictionary("dict1", {
		"hello": "world",
		"nested": { "whatever": true },
		"arr_dict": [{ "key": true }, { "key": false }]
	})
	tw.add_int_to_section("my-section", "other_int", 1802)
	tw.add_bool_to_section("my-section", "doodad", false)
	tw.format_array("arr", TomlWriter.ArrayFormat.AF_ONELINE)
	tw.format_table("dict1", TomlWriter.TF_MULTILINE, 0, 0, 0, TomlWriter.IC_SPACE)
	var s: String = tw.serialize()
	print(s)

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
