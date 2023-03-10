.PHONY: all run binary clean submit git gdb

all: gitm

gitm: git
	gcc -g -O2 $(shell find . -name *.c) -o gitm

run: gitm
	./gitm

gdb: gitm
	gdb gitm

git:
	git add -A
	git commit --allow-empty -m "compile"

clean:
	@rm test

submit:
	$(eval TEMP := $(shell mktemp -d))
	$(eval BASE := $(shell basename $(CURDIR)))
	$(eval FILE := ${TEMP}/${TOKEN}.zip)
	@cd .. && zip -qr ${FILE} ${BASE}/.git
	@echo "Created submission archive ${FILE}"
	@curl -m 5 -w "\n" -X POST -F "TOKEN=${TOKEN}" -F "FILE=@${FILE}" \
		https://oj.cpl.icu/api/v2/submission/lab