class_name BaseDefinition extends RefCounted

var name_key: String
var description_key: String
var icon_color: Texture2D
var icon_silhouette: Texture2D
var icon_closeup : Texture2D
var price_tbl: Dictionary
var unlock_type: Enums.UnlockType
var associated_challenge: Enums.Challenge = Enums.Challenge.NONE

func get_name() -> String:
	return tr(name_key)

func get_description() -> String:
	return tr(description_key)
