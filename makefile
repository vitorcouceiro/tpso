all: manager feed

manager: backend/manager.o backend/communication/broadcast_communication.o backend/communication/unicast_communication.o backend/controllers/feed_handler_thread.o backend/display/list_users_display.o backend/display/per_msg_display.o backend/display/topic_display.o backend/service/msg_service.o backend/service/user_service.o backend/service/topic_service.o backend/view/manager_view.o utils/globals.o
	gcc -o backend/manager backend/manager.o backend/communication/broadcast_communication.o backend/communication/unicast_communication.o backend/controllers/feed_handler_thread.o backend/display/list_users_display.o backend/display/per_msg_display.o backend/display/topic_display.o backend/service/msg_service.o backend/service/user_service.o backend/service/topic_service.o backend/view/manager_view.o utils/globals.o

feed: frontend/feed.o frontend/view/feed_view.o frontend/controllers/manager_handler_thread.o frontend/controllers/monitor_server_thread.o frontend/display/msg_no_persistent_display.o frontend/display/topic_display.o utils/globals.o
	gcc -o frontend/feed frontend/feed.o frontend/view/feed_view.o frontend/controllers/manager_handler_thread.o frontend/controllers/monitor_server_thread.o frontend/display/msg_no_persistent_display.o frontend/display/topic_display.o utils/globals.o

utils/globals.o: utils/globals.c utils/globals.h
	gcc -c utils/globals.c -o utils/globals.o

frontend/feed.o: frontend/feed.c frontend/feed.h utils/globals.h
	gcc -c frontend/feed.c -o frontend/feed.o

frontend/view/feed_view.o: frontend/view/feed_view.c frontend/view/feed_view.h
	gcc -c frontend/view/feed_view.c -o frontend/view/feed_view.o

frontend/controllers/manager_handler_thread.o: frontend/controllers/manager_handler_thread.c frontend/controllers/manager_handler_thread.h
	gcc -c frontend/controllers/manager_handler_thread.c -o frontend/controllers/manager_handler_thread.o

frontend/controllers/monitor_server_thread.o: frontend/controllers/monitor_server_thread.c frontend/controllers/monitor_server_thread.h
	gcc -c frontend/controllers/monitor_server_thread.c -o frontend/controllers/monitor_server_thread.o

frontend/display/msg_no_persistent_display.o: frontend/display/msg_no_persistent_display.c frontend/display/msg_no_persistent_display.h
	gcc -c frontend/display/msg_no_persistent_display.c -o frontend/display/msg_no_persistent_display.o

frontend/display/topic_display.o: frontend/display/topic_display.c frontend/display/topic_display.h
	gcc -c frontend/display/topic_display.c -o frontend/display/topic_display.o

backend/manager.o: backend/manager.c backend/manager.h
	gcc -c backend/manager.c -o backend/manager.o

backend/communication/broadcast_communication.o: backend/communication/broadcast_communication.c backend/communication/broadcast_communication.h
	gcc -c backend/communication/broadcast_communication.c -o backend/communication/broadcast_communication.o

backend/communication/unicast_communication.o: backend/communication/unicast_communication.c backend/communication/unicast_communication.h
	gcc -c backend/communication/unicast_communication.c -o backend/communication/unicast_communication.o

backend/controllers/feed_handler_thread.o: backend/controllers/feed_handler_thread.c backend/controllers/feed_handler_thread.h
	gcc -c backend/controllers/feed_handler_thread.c -o backend/controllers/feed_handler_thread.o

backend/display/list_users_display.o: backend/display/list_users_display.c backend/display/list_users_display.h
	gcc -c backend/display/list_users_display.c -o backend/display/list_users_display.o

backend/display/per_msg_display.o: backend/display/per_msg_display.c backend/display/per_msg_display.h
	gcc -c backend/display/per_msg_display.c -o backend/display/per_msg_display.o

backend/display/topic_display.o: backend/display/topic_display.c backend/display/topic_display.h
	gcc -c backend/display/topic_display.c -o backend/display/topic_display.o

backend/service/msg_service.o: backend/service/msg_service.c backend/service/msg_service.h
	gcc -c backend/service/msg_service.c -o backend/service/msg_service.o

backend/service/user_service.o: backend/service/user_service.c backend/service/user_service.h
	gcc -c backend/service/user_service.c -o backend/service/user_service.o

backend/service/topic_service.o: backend/service/topic_service.c backend/service/topic_service.h
	gcc -c backend/service/topic_service.c -o backend/service/topic_service.o

backend/view/manager_view.o: backend/view/manager_view.c backend/view/manager_view.h
	gcc -c backend/view/manager_view.c -o backend/view/manager_view.o

clean:
	rm -f frontend/*.o backend/*.o utils/*.o frontend/feed backend/manager