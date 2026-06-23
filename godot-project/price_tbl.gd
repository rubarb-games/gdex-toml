class_name PriceTbl extends RefCounted

static func encode(tbl: PriceTbl) -> Array[String]:
	return []

static func decode(list: Array[String]) -> PriceTbl:
	return null

static func enc_price_tbl(price_tbl: Dictionary) -> Array[String]:
	var price_list: Array[String]
	for gem_type: Enums.GemType in price_tbl:
		var value: int = price_tbl[gem_type]
		var currency: String
		match gem_type:
			Enums.GemType.RES_GOLD: currency = "gold"
			Enums.GemType.RES_COPPER: currency = "copper"
			Enums.GemType.RES_COAL: currency = "coal"
			_: currency = "gravel"
		price_list.append("%s:%d" % [currency, value])
	return price_list

static func dec_price_list(price_list: Array) -> Dictionary[Enums.GemType, int]:
	var tbl: Dictionary[Enums.GemType, int]
	for price: String in price_list:
		var colon_at: int = price.find(":")
		var currency: String = price.substr(0, colon_at)
		var value: int = price.substr(colon_at + 1).to_int()
		var gem_type: Enums.GemType
		match currency:
			"gold": gem_type = Enums.GemType.RES_GOLD
			"copper": gem_type = Enums.GemType.RES_COPPER
			"coal": gem_type = Enums.GemType.RES_COAL
			_: gem_type = Enums.GemType.GRAVEL
		tbl[gem_type] = value
	return tbl
