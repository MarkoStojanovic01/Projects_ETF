package space_force;
import java.awt.Color;
import java.awt.Graphics;

public abstract class Objekat {
	
	protected int x,y;
	protected Color boja;

	public void setX(int x) {
		this.x += x;
	}
	public void setY(int y) {
		this.y += y;
	}
	public Objekat(int x, int y, Color boja) {
		this.x = x;
		this.y = y;
		this.boja = boja;
	}
	
	public Objekat() {
		
	}
	public int getX() {
		return x;
	}
	public int getY() {
		return y;
	}
	
	public abstract void paint(Graphics g);
	
	public abstract boolean sadrziTacku(int i,int j);
}
