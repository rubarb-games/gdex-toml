class_name ObservableFloat extends RefCounted

signal changed(from_val: float, to_val: float)

var value: float

func _init(default_value: float) -> void:
	value = default_value

func add(val: float) -> void:
	var from_val: float = value
	value = value + val
	changed.emit(from_val, value)

func subtract(val: float) -> void:
	var from_val: float = value
	value = value - val
	changed.emit(from_val, value)

func set_value(val: float) -> void:
	var from_val: float = value
	value = val
	changed.emit(from_val, value)

func publish() -> void:
	changed.emit(value, value)
