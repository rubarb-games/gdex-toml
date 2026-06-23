class_name ObservableInt extends RefCounted

signal changed(from_val: int, to_val: int)

var value: int

func _init(default_value: int) -> void:
	value = default_value

func add(val: int) -> void:
	if val == 0:
		return
	var from_val: int = value
	value = value + val
	changed.emit(from_val, value)

func subtract(val: int) -> void:
	if val == 0:
		return
	var from_val: int = value
	value = value - val
	changed.emit(from_val, value)

func set_value(val: int) -> void:
	var from_val: int = value
	value = val
	changed.emit(from_val, value)

func publish() -> void:
	changed.emit(value, value)
