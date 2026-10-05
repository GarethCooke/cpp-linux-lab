LABS := $(sort $(wildcard labs/*/))

.PHONY: all check clean $(LABS)

all: $(LABS)

$(LABS):
	$(MAKE) -C $@

# Reference solutions under sanitizers: what CI runs
check:
	$(MAKE) -C labs/07-spsc-queue test IMPL=solution
	$(MAKE) -C labs/08-rule-of-five test IMPL=solution

clean:
	for d in $(LABS); do $(MAKE) -C $$d clean; done
