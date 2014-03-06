
.PHONY: tetris0.c tetris1.c tetris2.c imp.c movement0.c movement1.c block_remove0.c clean


samplessc: tetris0.c tetris1.c tetris2.c imp.c movement0.c movement1.c block_remove0.c
	rm -f samples.c
	cat *.c > samples.c

tetris0.c:
	python sampler.py < tetris0.csv > tetris0.c

tetris1.c:
	python sampler.py < tetris1.csv > tetris1.c

tetris2.c:
	python sampler.py < tetris2.csv > tetris2.c

imp.c:
	python sampler.py < imp.csv > imp.c

movement0.c:
	python sampler.py < movement0.csv > movement0.c
movement1.c:
	python sampler.py < movement1.csv > movement1.c

block_remove0.c:
	python sampler.py < block_remove0.csv > block_remove0.c

clean:
	rm *.c
	
