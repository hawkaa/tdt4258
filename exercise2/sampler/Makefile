
.PHONY: tetris0.c tetris1.c tetris2.c imp.c clean


tetris.c: tetris0.c tetris1.c tetris2.c
	cat tetris0.c tetris1.c tetris2.c > tetris.c

tetris0.c:
	python sampler.py < tetris0.csv > tetris0.c

tetris1.c:
	python sampler.py < tetris1.csv > tetris1.c

tetris2.c:
	python sampler.py < tetris2.csv > tetris2.c

imp.c:
	python sampler.py < imp.csv > imp.c

clean:
	rm *.c
	
