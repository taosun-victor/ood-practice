import java.util.Arrays;

public class MyHashMap<K, V> {
	public static class Node<K, V> {
		private final K key;
		private V value;
		Node<K, V> next = null;
		
		public Node(K k, V v) {
			key = k;
			value = v;
		}
		
		public K getKey() {
			return key;
		}
		
		public V getValue() {
			return value;
		}
		
		public void setValue(V v) {
			value = v;
		}
	}
	
	private static final int INIT_CAP = 10;
	private static final double LOAD_FACTOR = 0.7;
	
	private Node<K, V> [] array;
	private int size;
	
	public MyHashMap() {
		array = (Node<K, V>[]) new Node[INIT_CAP];
		size = 0;
	}
	
	public int size() {
		return size;
	}
	
	public boolean isEmpty() {
		return size == 0;
	}
	
	public void clear() {
		Arrays.fill(array, null);
		size = 0;
	}
	
	
	
	// --------------- HashMap related functionalities ---------------
	public V get(K key) {
		Node<K, V> node = array[index(key)];
		while(node != null) {
			if (equalsKey(node.getKey(), key)) {
				return node.getValue();
			}
			node = node.next;
		}
		return null;
	}
	
	public boolean containsKey(K key) {
		Node<K, V> node = array[index(key)];
		while(node != null) {
			if (equalsKey(node.getKey(), key)) {
				return true;
			}
			node = node.next;
		}
		return false;
	}
	
	public V put(K key, V value) {
		int i = index(key);
		Node<K, V> node = array[i];
		
		// try to update if key already exist in hashMap
		while(node != null) {
			if (equalsKey(node.getKey(), key)){
				V oldValue = node.getValue();
				node.setValue(value);
				return oldValue;
			}
			node = node.next;
		}
		
		// add new entry if not found
		Node<K, V> newEntry = new Node<>(key, value);
		newEntry.next = array[i];
		array[i] = newEntry;
		size++;
		
		if(needRehashing()) {
			rehash();
		}
		return null;
	}
	
	
	public V remove(K key) {
		int i = index(key);
		Node<K, V> prev = null;
		Node<K, V> curr = array[i];
		
		while(curr != null) {
			if (equalsKey(curr.getKey(), key)) {
				if (prev == null) {
					array[i] = curr.next;
				}
				else {
					prev.next = curr.next;
				}
				size--;
				return curr.getValue();
			}
			
			prev = curr;
			curr = curr.next;
		}
		return null;
	}
	
	// ----------------- APIs to realize the hashMap --------------
	private int hash(K key) {
		if (key == null) 
			return 0;
		
		int code = key.hashCode();
		return code & 0x7fffffff;
	}
	
	private int index(K key) {
		return hash(key) % array.length;
	}
	
	private boolean equalsKey(K a, K b) {
		return a == b || a != null && a.equals(b);
	}
	
	private boolean needRehashing() {
		return size > LOAD_FACTOR * array.length;
	}
	
	private void rehash() {
		Node<K, V>[] old = array;
		array = (Node<K, V>[]) new Node[old.length * 2];
		for (Node<K, V> e : old) {
			while(e != null) {
				Node<K, V> next = e.next;
				
				int i = index(e.getKey());
				e.next = array[i];
				array[i] = e;
				
				e = next;
			}
		}
	}
}
