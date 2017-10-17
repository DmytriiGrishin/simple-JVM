public class Test{
	public static void main(String[] agrs){
		int a = 15;
		int b=1 ;
		for(int i=0; i<10; i++){
			b = sum(a, b);
		}
	}
	public static int sum(int a, int b){
		return a+b;
	}
}

