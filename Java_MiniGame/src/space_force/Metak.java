package space_force;

import java.awt.Color;
import java.awt.Graphics;

public class Metak {

	private int x,y1,y2,duzina;
	
	public Metak(int x,int y1,int d) {
		this.x = x;
		this.y1 = y1;
		this.duzina = d;
		this.y2 = y1 - d;
	}
	
	public int getX() {
		return x;
	}
	public int getY() {
		return y2;
	}
	
	public int getDuzina() {
		return duzina;
	}
	
	void pomeri(int y) {
		y1 -= y;
		y2 -= y;
	}
	
	public void paint(Graphics g) {
		Color prevColor = g.getColor();
		g.setColor(Color.WHITE);
		g.drawLine(x, y1, x, y2);
		g.setColor(prevColor);
	}
	
}
