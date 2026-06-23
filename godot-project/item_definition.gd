class_name ItemDefinition extends BaseDefinition

var tier: Enums.ItemTier
var item_type: Enums.Item
var scene_path: String
var script_path: String
var atlas_pos: Vector2i

var fx_variation: int #This is used to pick the correct fx for the item to be used
var item_fx: String

func get_tooltip() -> String:
	return "[b]%s[/b]\n%s" % [get_name(), get_description()]
