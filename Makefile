all: sender receiver timed_sender timed_receiver

sender:
	gcc simple_queue/sender.c -lrt -o simple_queue/sender

receiver:
	gcc simple_queue/receiver.c -lrt -o simple_queue/receiver

timed_sender:
	gcc timeout_queue/sender.c -lrt -o timeout_queue/sender

timed_receiver:
	gcc timeout_queue/receiver.c -lrt -o timeout_queue/receiver

clean:
	rm simple_queue/sender simple_queue/receiver

run:
	@echo ---------------------
	@echo Running simple_queue
	@echo ---------------------
	./simple_queue/receiver &
	./simple_queue/sender
	@echo ---------------------
	@echo Running timed_queue
	@echo ---------------------
	./timeout_queue/receiver &
	./timeout_queue/sender
	@echo ---------------------
	@echo Running poll_queue
	@echo ---------------------
	@echo --- END ---
