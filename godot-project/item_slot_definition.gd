class_name ItemSlotDefinition extends RefCounted

var tier: Enums.ItemTier
var price_tbl: Dictionary[Enums.GemType, int]
var equipped: Enums.Item
var state: Enums.ItemSlotState
var locked_by: Enums.Item

func copy(other: ItemSlotDefinition) -> ItemSlotDefinition:
	var d: ItemSlotDefinition = ItemSlotDefinition.new()
	d.tier = other.tier
	d.price_tbl = other.price_tbl.duplicate()
	d.equipped = other.equipped
	d.state = other.state
	d.locked_by = other.locked_by
	return d
