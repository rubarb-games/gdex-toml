class_name Utils extends RefCounted

static func get_bits(number: int, max_bits: int = 64, space_nth: int = 8) -> String:
	var binary_string: String = ""
	# Iterate from the most significant bit (63rd for a 64-bit int) down to 0
	for i in range(max_bits-1, -1, -1):
		# Check if the i-th bit is set (1) or not (0)
		if (number >> i) & 1:
			binary_string += "1"
		else:
			binary_string += "0"
		# Add a space for readability after every 8 bits (optional)
		if i % space_nth == 0 and i != 0:
			binary_string += " "
	return binary_string

static func to_v2(v: Vector3) -> Vector2:
	return Vector2(v.x, v.y)

static func v2(x: float, y: float) -> Vector2:
	return Vector2(x, y)

static func to_v3(v: Vector2, z: float = 0.0) -> Vector3:
	return Vector3(v.x, v.y, z)

static func v3(x: float, y: float, z: float = 0.0) -> Vector3:
	return Vector3(x, y, z)

static func shuffle(arr: Array, rand_gen: RandomNumberGenerator) -> void:
	for i in range(arr.size() - 1, 0, -1):
		var j : int = rand_gen.randi_range(0, i)
		var temp = arr[i]
		arr[i] = arr[j]
		arr[j] = temp

static func coord_str(x: int, y: int) -> String:
	return "(%-2d, %-2d)" % [x, y]

static func v2_str(v: Vector2) -> String:
	return "(%-2d, %-2d)" % [v.x, v.y]

static func v2i_str(v: Vector2i) -> String:
	return "(%-2d, %-2d)" % [v.x, v.y]

static func v3_str(v: Vector3) -> String:
	return "(%-2d, %-2d, %-2d)" % [v.x, v.y, v.z]


static func load_toml_file(p_parser: TomlParser, p_paths: Array[String]) -> int:
	for path: String in p_paths:
		var file = FileAccess.open(path, FileAccess.READ)
		if file == null:
			#print("Utils.load_toml_file :: Coult not find %s" % path)
			continue
		var content = file.get_as_text()
		if p_parser.try_parse(content):
			#print("Utils.load_toml_file :: loaded and parsed toml file: "+path)
			return OK
	var feedback: String = "Utils.load_toml_file :: Could not find a toml file in any of the following paths:\n"
	for path: String in p_paths:
		feedback += " - %s\n" % path
	print(feedback)
	return ERR_CANT_OPEN

static func write_file(contents: String, file_path: String) -> void:
	var file: FileAccess = FileAccess.open(file_path, FileAccess.WRITE)
	if not file:
		print("Utils.write_file :: Failed to write %s" % [file_path])
		return
	
	file.store_string(contents)
	file.close()

static func thousand_delimiter(n : int) -> String:
	if n < 1000:
		return str(n)

	var num:String = str(n)
	var count:int = 0

	for i in range(num.length() - 1, 0, -1):
		count = count + 1
		if count % 3 == 0:
			num = num.insert(i, ",")

	if num[0] == ",":
		num = num.substr(1)

	return num

static func bool_str(b: bool) -> String:
	return "true" if b else "false"

static func yesno_str(b: bool) -> String:
	return "Yes" if b else "No"

static func remove_duplicate_ints(original_array: Array[int]) -> Array[int]:
	var unique_array: Array[int] = []
	for item in original_array:
		if not unique_array.has(item):
			unique_array.append(item)
	return unique_array

static func str_join(p_arr: Array[String], p_delimeter: String) -> String:
	var s: String = ""
	for i: int in range(0, p_arr.size()):
		s += p_arr[i]
		if i < p_arr.size() - 1:
			s += p_delimeter
	return s

static func unixtime_to_str(unix: int) -> String:
	var time: Dictionary = Time.get_datetime_dict_from_unix_time(unix)
	return "%s %02d.%02d - %02d:%02d:%02d" % [time.year, time.day, time.month, time.hour, time.minute, time.second]

static func get_time_since(unix_old: int) -> String:
	var time: Dictionary = Time.get_datetime_dict_from_system()
	var unix_cur: int = Time.get_unix_time_from_datetime_dict(time)
	return get_hms_str(unix_cur - unix_old)

static func get_hms_str(unix: int) -> String:
	var h: int = int(float(unix) / 3600.0) % 24
	var m: int = int(float(unix) / 60.0) % 60
	var s: int = unix % 60
	return "%02d:%02d:%02d" % [h, m, s]
