package space_force;

import java.awt.Canvas;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.ArrayList;

public class Svemir extends Canvas implements Runnable{

	private Simulator owner;
	private Thread thread;
	private ArrayList<Nebesko_telo> nebeska = new ArrayList<>();
	private Igrac igrac;
	private ArrayList<Metak> meci = new ArrayList<>();
	private int score = 0;
	
	public Svemir(Simulator owner) {
		setBackground(Color.BLACK);
		this.owner = owner;
	}
	
	public void zavrsi() {
		if (thread != null) thread.interrupt();
	}
	
	public void dodatiNebeskoTelo(Nebesko_telo t) {
		nebeska.add(t);
	}
	
	void pokreniSvemir() {
		synchronized(thread) {
		thread.notify();
		}
	}

	@Override
	public void run() {
		
		Graphics g = this.getGraphics();
		try {
			synchronized(thread) {
			thread.wait();
			}
			while(!thread.isInterrupted()) {
				Thread.sleep(100);
				super.paint(g);
				synchronized(thread) {
					provera();
				}
				synchronized(thread) {
					igrac.paint(g);
					for(Nebesko_telo t:nebeska) {
						t.paint(g);
						t.setY(5);
					}
					int brm = meci.size();
					for(int i=0;i<brm;i++) {
						meci.get(i).paint(g);
						meci.get(i).pomeri(meci.get(i).getDuzina());
					}
					azuriraj();
				}
				
				
			}
		} catch(InterruptedException e) {}
		
	}
	
	public void provera() {
		for(Nebesko_telo t:nebeska) {
			if(t.sadrziTacku(igrac.getX(), igrac.getY())) {
				Simulator s = (Simulator)(getParent());
				s.gotovaIgra();
			};
		}
	}
	
	public void azuriraj() {
		if (nebeska.isEmpty()) return;
		int size = nebeska.size();
		//int k=0;
		//int s=0;
		for(int i=0;i<size;i++) {
			int brm = meci.size();
			for(int j=0;j<brm;j++) {
				if(nebeska.get(i).sadrziTacku(meci.get(j).getX(), meci.get(j).getY())) {
					unistiKometu(nebeska.get(i));
					i--;
					size--;
					break;
				}
			}
		}
		
	}
	public void unistiKometu(Nebesko_telo k) {
		if (k.getR() < 20) nebeska.remove(k);
		else {
			Kometa kometa1 = new Kometa(k.getX()-k.getR(),k.getY(),10);
			Kometa kometa2 = new Kometa(k.getX()+k.getR(),k.getY(),10);
			nebeska.remove(k);
			nebeska.add(kometa1);
			nebeska.add(kometa2);
		}
		score += 20;
		Simulator s = (Simulator)(getParent());
		s.score.setText("" + score);
		s.score.revalidate();
	}
	
	public void paint(Graphics g) {
		zavrsi();
		thread = new Thread(this);
		thread.start();
	}
	
	public void postaviIgraca(Igrac i) {
		igrac = i;
	}
	
	public void pomeriIgraca(int x) {
		igrac.setX(x);
	}
	
	public void dodajMetak() {
		meci.add(new Metak(igrac.getX(),igrac.getY(),8));
	}
	
}
