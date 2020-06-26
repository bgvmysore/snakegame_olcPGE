#define OLC_PGE_APPLICATION
#include "olcPGE/olcPixelGameEngine.h"

struct fruit {
	int Fx, Fy;
	
	void genFruit(int xx, int yy) {
		std::srand(std::time(NULL));
		Fx = std::rand();
		Fy = std::rand();
		Fx = (Fx % (xx-3))+3;
		Fy = (Fy % (yy-3))+3;
	}

	bool HannIde(int xx,int yy) {
		if (Fx == xx && Fy == yy)
			return true;
		else
			return false;
	}

	int getHannX() const {
		return this->Fx;
	}

	int getHannY() const {
		return this->Fy;
	}

};

class worm {
	int x=0, y=0;
	int speed = 1;
	char direction[4] = {'N','E','S','W'};
	short nBala = 0;
	int BalaX[100];
	int BalaY[100];
	bool incBala = false;
public:
	char currentDirection = NULL;

	int getBalaCount() const { return nBala; }

	void incrementBala() { 
		this->nBala += 1;
		this->incBala = true;
	}

	void setPositionInit(int xg,int yg) {
		x = xg;
		y = yg;
		this->currentDirection = NULL;
		for (int j = 0; j < 100; j++) {
			this->BalaX[j] = 0;
			this->BalaY[j] = 0;
		}
		nBala = 5;
		BalaX[0] = xg;
		BalaY[0] = yg + 1;
		BalaX[1] = xg;
		BalaY[1] = yg + 2;
		BalaX[2] = xg;
		BalaY[2] = yg + 3;
		BalaX[3] = xg;
		BalaY[3] = yg + 4;
		BalaX[4] = xg;
		BalaY[4] = yg + 5;
	}

	void setPosition(int xg, int yg) {
		x = xg;
		y = yg;
	}
	
	int getPosX() const { return x; }
	int getPosY() const { return y; }

	int getSpeed() const {
		return this->speed;
	}

	void setSpeed(int sp) {
		speed = sp;
	}

	void updatePos(char xc) {
		int xb1, yb1;
		int xb2, yb2;

		xb1 = BalaX[0];
		yb1 = BalaY[0];
		BalaX[0] = x;
		BalaY[0] = y;

		if (xc == 'N')
			y+=speed;
		else if (xc == 'S')
			y-=speed;
		else if (xc == 'E')
			x+=speed;
		else
			x-=speed;

		int xl1 = BalaX[nBala - 2];
		int yl1 = BalaY[nBala - 2];
		
		for (int k = 1; k < nBala; k++) {
			xb2 = BalaX[k];
			yb2 = BalaY[k];
			BalaX[k] = xb1;
			BalaY[k] = yb1;
			xb1 = xb2;
			yb1 = yb2;
		}

		if (this->incBala) {
			this->incBala = false;
			BalaX[nBala - 1] = xl1;
			BalaY[nBala - 1] = yl1;
		}

	}
	
	char North() const { return 'N'; }
	char South() const { return 'S'; }
	char East() const { return 'E'; }
	char West() const { return 'W'; }

	bool isHere(int xp,int yp) {
		if (xp == int(x) && yp == int(y))
			return true;
		else
			return false;
	}

	bool BalaIde(int xp, int yp) {
		for (int i = 0; i < nBala; i++) {
			if (xp == BalaX[i] && yp == BalaY[i])
				return true;
		}
		return false;
	}
};

// Override base class with your custom functionality
class Example : public olc::PixelGameEngine
{
	fruit h1;
	worm s1;
	float f1 = 0.0f;
	bool updateframe = true;
	
public:
	Example()
	{
		// Name you application
		sAppName = "SnakeGame";
	}

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		s1.setPositionInit(ScreenWidth() / 2, ScreenHeight() / 2);
		h1.genFruit(ScreenWidth(), ScreenHeight());
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// called once per frame, draws random coloured pixels
		this->f1 += fElapsedTime;
		
		//Generate Fruit Logic
		bool HannInBala = false;
		if (s1.getPosX() == h1.getHannX() && s1.getPosY() == h1.getHannY()) {
			s1.incrementBala();
			do {
				h1.genFruit(ScreenWidth(), ScreenHeight());
				HannInBala = s1.BalaIde(h1.Fx, h1.Fy) || (h1.Fx == 0) || (h1.Fx == 1)
					|| (h1.Fx == ScreenWidth()-1) || (h1.Fx == ScreenWidth()-2)
					|| (h1.Fy == 0) || (h1.Fy == 1) || (h1.Fy == ScreenHeight() -1)
					|| (h1.Fy == ScreenHeight() -2);
			} while (HannInBala);
		}

		//Get Key Strokes
		if (GetKey(olc::Key::LEFT).bPressed && s1.currentDirection != s1.East())s1.currentDirection = s1.West();
		else if (GetKey(olc::Key::RIGHT).bPressed && s1.currentDirection != s1.West()) s1.currentDirection = s1.East();
		else if (GetKey(olc::Key::UP).bPressed && s1.currentDirection != s1.North()) s1.currentDirection = s1.South();
		else if (GetKey(olc::Key::DOWN).bPressed && s1.currentDirection != s1.South()) s1.currentDirection = s1.North();
		
		//Speed 1ms = 0.1f
		if (f1 > 0.1f) {
			f1 = 0;

			if (s1.currentDirection == s1.West())
				s1.updatePos(s1.West());
			if (s1.currentDirection == s1.East())
				s1.updatePos(s1.East());
			if (s1.currentDirection == s1.South())
				s1.updatePos(s1.South());
			if (s1.currentDirection == s1.North())
				s1.updatePos(s1.North());

			//Snake eat itself
			if (s1.BalaIde(s1.getPosX(), s1.getPosY())) {
				this->OnUserCreate();
			}
		}

		//Out of Bound Reset
		if (int(s1.getPosX()) == 0 || int(s1.getPosX()) == 1)
			s1.setPosition(ScreenWidth()-3,s1.getPosY());
		if (int(s1.getPosX()) == ScreenWidth()-2 || int(s1.getPosX()) == ScreenWidth() -1)
			s1.setPosition(2, s1.getPosY());
		if (int(s1.getPosY()) == 0 || int(s1.getPosY()) == 1)
			s1.setPosition(s1.getPosX(), ScreenHeight() - 3);
		if (int(s1.getPosY()) == ScreenHeight() - 2 || int(s1.getPosY()) == ScreenHeight() - 1)
			s1.setPosition(s1.getPosX(), 2);

		//Render Screen
		int tempVar = 0;
		for (int x = 0; x < ScreenWidth(); x++)
			for (int y = 0; y < ScreenHeight(); y++)
				if(y == 0 || y == ScreenHeight()-1 || y==1 || y == ScreenHeight() - 2)
					Draw(x, y, olc::Pixel(olc::BLACK));
				else if (x == 0 || x == ScreenWidth()-1 || x == 1 || x == ScreenWidth() - 2)
					Draw(x, y, olc::Pixel(olc::BLACK));
				else if(h1.HannIde(x,y))
					Draw(x, y, olc::Pixel(olc::MAGENTA));
				else if (s1.isHere(x,y))
					Draw(x, y, olc::Pixel(olc::YELLOW));
				else if (s1.BalaIde(x, y)) {
					
					if (tempVar % 3 == 2)
					{
						Draw(x, y, olc::Pixel(olc::GREEN));
						tempVar++;
					}
					else if(tempVar % 3 == 1) 
					{
						Draw(x, y, olc::Pixel(olc::VERY_DARK_GREEN));
						tempVar++;
					}
					else
					{	
						Draw(x, y, olc::Pixel(olc::DARK_GREEN));
						tempVar++;
					}
				}
				else
					Draw(x, y, olc::Pixel(olc::GREY));			
		return true;
	}
};

int main()
{
	Example demo;
	if (demo.Construct(48, 48, 8, 8))
		demo.Start();
	return 0;
}