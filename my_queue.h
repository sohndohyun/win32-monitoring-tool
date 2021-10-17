#ifndef __MY_QUEUE_H__
#define __MY_QUEUE_H__

template<typename type>
class my_queue {
private:
	int _qmax;
	type* _queue;
	int _cnt, _front, _rear;
public:
	my_queue(int qmax = 50) {
		_queue = new type[qmax];
		_cnt = 0;
		_front = 0;
		_rear = 0;
		_qmax = qmax;
	}

	int getCount() { return _cnt; }

	int enqueue(type _data) {
		_rear = (_rear + 1) % _qmax;
		if (_front == _rear) {
			_rear = (_rear - 1) % _qmax;
			return false;
		}
		_cnt++;
		_queue[_rear] = _data;
		return true;
	}
	int dequeue(type* _data) {
		if (_front == _rear) return false;
		_cnt--;
		(*_data) = _queue[_front];
		_front = (_front + 1) % _qmax;
		return true;
	}

	int peek(int num, type* _data) {
		if (_qmax <= num) return false;
		if (_cnt <= num) return false;
		*_data = _queue[(_front + num + 1) % _qmax];
		return true;
	}

	~my_queue() { delete[] _queue; }
};
#endif
