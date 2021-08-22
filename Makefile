all: sender receiver timed_sender timed_receiver poll_sender poll_receiver

sender:
	gcc simple_queue/sender.c -lrt -o simple_queue/sender

receiver:
	gcc simple_queue/receiver.c -lrt -o simple_queue/receiver

timed_sender:
	gcc timeout_queue/sender.c -lrt -o timeout_queue/sender

timed_receiver:
	gcc timeout_queue/receiver.c -lrt -o timeout_queue/receiver

poll_sender:
	gcc poll_queue/sender.c -lrt -o poll_queue/sender

poll_receiver:
	gcc poll_queue/receiver.c -lrt -o poll_queue/receiver

clean:
	rm simple_queue/sender simple_queue/receiver
	rm timeout_queue/sender timeout_queue/receiver
	rm poll_queue/sender poll_queue/receiver

run: all
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
	./poll_queue/receiver &
	./poll_queue/sender
	@echo --- END ---
