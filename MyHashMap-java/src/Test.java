
public class Test {
	public static void main(String[] args) {
		MyHashMap<Coordinate, String> map = new MyHashMap();
		
		Coordinate coord1 = new Coordinate(1, 3);
		Coordinate coord2 = new Coordinate(2, 4);
		Coordinate coord3 = new Coordinate(3, 3);
		Coordinate coord4 = new Coordinate(1, 3); // same as coord1
		Coordinate coord5 = new Coordinate(5, 3);
		Coordinate coord6 = new Coordinate(20, 3);
		Coordinate coord7 = new Coordinate(100, 3);
		Coordinate coord8 = new Coordinate(2, 40);
		Coordinate coord9 = new Coordinate(36, 30);
		Coordinate coord10 = new Coordinate(101, 3);
		Coordinate coord11 = new Coordinate(2, 34);
		Coordinate coord12 = new Coordinate(35, 39);
		
		map.put(coord1, "Folks");
		map.put(coord2, "NRA");
		map.put(coord3, "Jacobi");
		System.out.println(map.get(coord4));
		
		map.put(coord4, "UIUC ME");
		System.out.println(map.get(coord1));
		
		map.put(coord5, "Lion");
		map.put(coord6, "Lamb");
		System.out.println(map.size());
		
		map.put(coord7, "Some");
		map.put(coord8, "Random");
		map.put(coord9, "Things");
		map.put(coord10, "No clue what it is");
		map.put(coord11, "How is life");
		map.put(coord12, "Life is tough");
		System.out.println(map.size());
		System.out.println(map.get(coord12));
		
		map.remove(coord12);
		System.out.println(map.size());
		System.out.println(map.get(coord12));
	}
}
