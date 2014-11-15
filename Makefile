test:
	./node_modules/.bin/mocha tests/* --reporter list

tdd:
	./node_modules/.bin/mocha -w -t 1000 tests/* --reporter list

.PHONY: test
