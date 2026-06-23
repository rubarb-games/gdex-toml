class_name TownConfig extends RefCounted

var id: String
var key_name: String
var prev_town: String # id of prev town
var next_town: String # id of next town

func get_name() -> String:
	return tr(key_name)
