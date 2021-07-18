bigoutput littleoutput:
#1.use "" to avoid bash translate <, note = is not metacharacters so it will not be interpreted.\
the 3rd can't substitue successfully since $@ is not output\
if bigoutput=bigoutput, then the 4th line is big, empty otherwise.\
all others can output big if target is bigoutput
	@echo "[$(patsubst %output,%,$@)]<=$@"
	@echo "[$(subst output,,$@)]"
	@echo "[$(patsubst output,,$@)]"
	@echo "[$(bigoutput:output=)]"
	@echo "[$(@:output=)]"