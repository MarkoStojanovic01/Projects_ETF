package space_force;

import java.awt.Color;
import java.awt.Graphics;

public abstract class Nebesko_telo extends Objekat {

	protected int r;

	public Nebesko_telo(int x, int y, Color boja, int r) {
		super(x, y, boja);
		this.r = r;
	}
	
	public Nebesko_telo() {}
	
	@Override
	public abstract boolean sadrziTacku(int i,int j);

	@Override
	public abstract void paint(Graphics g);

	public int getR() {
		return r;
	}
	
	

}
