package space_force;

public class Generator extends Thread {

	private Svemir svemir;
	
	public Generator(Svemir s) {
		svemir = s;
	}
	
	public void pokreni() {
		this.start();
	}
	
	public void zavrsi() {
		if(this != null) this.interrupt();
	}
	
	public void run() {
		try {
			while(!isInterrupted()) {
				sleep(900);
				int x = (int)(Math.random()*200);
				int y = 0;
				int r = 10 + (int)(Math.random()*20);
				synchronized(this) {
				svemir.dodatiNebeskoTelo(new Kometa(x,y,r));
				}
			}
		} catch(InterruptedException e) {}
	}
}
