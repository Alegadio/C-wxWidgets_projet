.PHONY: clean All

All:
	@echo "----------Building project:[ TP3 - Debug ]----------"
	@cd "TP3" && "$(MAKE)" -f  "TP3.mk"
clean:
	@echo "----------Cleaning project:[ TP3 - Debug ]----------"
	@cd "TP3" && "$(MAKE)" -f  "TP3.mk" clean
