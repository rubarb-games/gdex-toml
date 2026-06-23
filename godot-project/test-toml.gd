extends Node

func _ready() -> void:
	var cfg: GameConfig = GameConfig.new()
	GameConfigIO.load_config(cfg)
	print("done")
	
