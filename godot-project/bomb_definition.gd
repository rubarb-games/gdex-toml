class_name BombDefinition extends BaseDefinition

var category: Enums.BombCategory
var bomb_type: Enums.GemType
var script_path: String
var atlas_pos: Vector2i

func get_tooltip() -> String:
	return "[b]%s[/b]\n%s" % [get_name(), get_description()]
