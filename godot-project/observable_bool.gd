class_name ObservableBool extends RefCounted

signal changed(from_val: bool, to_val: bool)

var value: bool

func _init(default_value: bool) -> void:
	value = default_value

func set_value(val: bool) -> void:
	if val == value:
		return
	var from_val: bool = value
	value = val
	changed.emit(from_val, value)

func publish() -> void:
	changed.emit(value, value)
