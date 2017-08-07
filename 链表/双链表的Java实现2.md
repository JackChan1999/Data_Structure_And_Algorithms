## 双链表的Java实现
```java
public class MyDoubleLink implements Iterable<Object>{
	private class Node{
		public Node(Object data){
			this.data = data;
		}
		Node next;
		Node prev;
		Object data;
	}
	private Node head;
	private Node rear;
	
	public void add(Object data){
		Node node = new Node(data);
		if (head == null){
			head = node;
			rear = node;
		} else {
			rear.next = node;
			node.prev = rear;
			rear = node;
		}
	}
	
	public boolean contains(Object data){
		Node node = find(data);
		return node != null;
	}
	
	public void print(){
		Node temp = head;
		while(temp != null){
			System.out.print(temp.data + ",");
			temp = temp.next;
		}
		System.out.println();
	}
	private Node find(Object data){
		Node node = head;
		while (node != null){
			if (node.data.equals(data) && 
                node.data.hashCode() == data.hashCode()){
				break;
			} else {
				node = node.next;
			}
		}
		return node;
	}
	
	public void remove(Object data){
		Node node = find(data);
		if (node != null){
			if (node == head && node == rear){//只有一个节点
				head = null;
				rear = null;
			} else if (node == head){ //头节点
				head = head.next;
				head.prev = null;
			} else if (node == rear){ //尾节点
				rear = rear.prev;
				rear.next = null;
			} else { //中间节点
				node.prev.next = node.next;
				node.next.prev = node.prev;
			}
		}
	}

	@Override
	public Iterator<Object> iterator() {
		Iterator<Object> ite = new Iterator<Object>() {
			private Node temp = head;
			@Override
			public boolean hasNext() {
				return temp != null;
			}

			@Override
			public Object next() {
				Object data = temp.data;
				temp = temp.next;
				return data;
			}

			@Override
			public void remove() {
			}
		};
		return ite;
	}	
}
```
## 测试双链表
```java
public class Test {
	
	public static  class Instance{
		public Instance(int i){
			
		}
		public Instance(){
			
		}
	}

	public static void main(String[] args) {
		Instance in  = new Instance(){
			
		};
		MyDoubleLink datas = new MyDoubleLink();
		datas.add("aaa");
		datas.add("bbb");
		datas.add("ccc");
		datas.print();
		datas.remove("ccc");
		datas.print();
		for (Object d : datas) {
			System.out.println(d);
		}
	}
}
```