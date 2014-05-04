from gevent import monkey, spawn

monkey.patch_all()

import os
import json
from sseclient import SSEClient
from geventwebsocket import WebSocketServer, WebSocketApplication, Resource

clients = []


def sse_handler():
    events = SSEClient(
        url='https://api.spark.io/v1/events/game',
        headers={'Authorization': 'Bearer {}'.format(os.environ['ACCESS_TOKEN'])},
    )
    for event in events:
        if event.event == 'game':
            data = json.loads(event.data)
            print('event: {}'.format(data['data']))
            for client in clients:
                client.send(data['data'])


class SnakeApp(WebSocketApplication):

    def on_open(self):
        print('new client')
        clients.append(self.ws)

    def on_message(self, data):
        pass

    def on_close(self, reason):
        print('client disconnect')
        clients.remove(self.ws)


if __name__ == '__main__':
    spawn(sse_handler)
    WebSocketServer(
        ('', 9000),
        Resource({'/': SnakeApp})
    ).serve_forever()
