extends Node

func _ready() -> void:
	var file_path: String = "res://data/game_config.toml"
	var g_conf: String = load_text_file(file_path)
	var tp: TomlParser = TomlParser.new()
	tp.logging(true)
	if not tp.try_parse(g_conf):
		push_error("Cannot parse TOML %s" % [file_path])
	
	print("TomlParser.key_exists")
	print("Key 'audio' exists: %s" % yes_no(tp.key_exists("audio")))
	print("Key 'undefined' exists: %s" % yes_no(tp.key_exists("undefined")))
	print("\n")
	
	print("TomlParser.path_exists")
	print("Path 'colors/red' exists: %s" % yes_no(tp.path_exists(["colors", "red"])))
	print("Path 'colors/undefined' exists: %s" % yes_no(tp.path_exists(["colors", "undefined"])))
	print("\n")
	
	print("Strings")
	print(" - get_string: %s" % tp.get_string("string_single"))
	print(" - get_string_or: %s" % tp.get_string_or("string_undefined", "Default string"))
	print(" - get_string_at: %s" % tp.get_string_at(["strings", "single"]))
	print(" - get_string_at_or: %s" % tp.get_string_at_or(["strings", "undefined"], "Default string at"))
	print(" - get_string_arr: %s" % to_str_arr(tp.get_string_arr("string_multiple")))
	print(" - get_string_arr_at: %s" % to_str_arr(tp.get_string_arr_at(["strings", "multiple"])))
	print("\n")
	
	# int
	# float
	# vec2
	# vec2i
	# vec3
	# vec3i
	# color
	# table
	# array
	
	print("Has key undefined? %s" % [yes_no(tp.key_exists("undefined"))])
	tp.get_table("undefined")
	
	print("Has key audio? %s" % [yes_no(tp.key_exists("audio"))])
	
	#print("Has keys [audio, volume_master]? %s" % [yes_no(tp.path_exist(["audio", "volume_master"]))])
	#print("Has keys [audio, undefined]? %s" % [yes_no(tp.path_exist(["audio", "undefined"]))])
	
	var audio: Dictionary = tp.get_table("audio")
	print(audio)
	var audio_keys: Array[String] = tp.get_table_keys("audio")
	print(audio_keys)
	var town_first: Dictionary = tp.get_table_at(["town", "first"])
	print(town_first)

	var col_undefined: Color = tp.get_color("color_undefined")
	print("Color undefined: %s" % [col_undefined])
	var col_single: Color = tp.get_color("color_single")
	print("Color single: %s" % [col_single])
	var col_rgb: Color = tp.get_color("color_rgb")
	print("Color rgb: %s" % [col_rgb])

	var col_or_undef: Color = tp.get_color_or("color_undefined", Color(1.0, 1.0, 0.0))
	print("Color or undef: %s" % [col_or_undef])
	var col_or: Color = tp.get_color_or("color_single", Color(1.0, 1.0, 0.0))
	print("Color or: %s" % [col_or])

	var col_at_red: Color = tp.get_color_at(["colors", "red"])
	print("Color at colors/red: %s" % [ col_at_red ])
	var col_at_gui_btn: Color = tp.get_color_at(["colors", "gui", "button"])
	print("Color at colors/gui/button: %s" % [ col_at_gui_btn ])

	var col_multiple: Array[Color] = tp.get_color_arr("color_multiple")
	for i: int in range(col_multiple.size()):
		print("col[%d]: %s" % [i, col_multiple[i]])

	var col_hud_labels: Array[Color] = tp.get_color_arr_at(["colors", "hud", "labels"])
	for i: int in range(col_hud_labels.size()):
		print("col hud label [%d]: %s" % [i, col_hud_labels[i]])

func to_str_arr(arr: Array[String]) -> String:
	var res: String = "["
	for i: int in range(arr.size()):
		res += arr[i]
		if i < arr.size() - 1:
			res += ", "
	res += "]"
	return res

func yes_no(val: bool) -> String:
	return "yes" if val else "no"

func load_text_file(file_path: String) -> String:
	if not FileAccess.file_exists(file_path):
		push_error("File does not exist: " + file_path)
		return ""
	
	var file = FileAccess.open(file_path, FileAccess.READ)
	var content = file.get_as_text()
	file.close()
	return content
