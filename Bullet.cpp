#include "Bullet.h"
#include <vector>

#include "Structs.h"

using namespace std;

BulletHandler::BulletHandler(){


}

bool BulletHandler::MakeBullet(float nx, float ny, bool Flipped){

        //cout<<endl<<IsFlipped;
        BulletCoord TempBullet{nx,ny,Flipped};
        Bulletarray.push_back(TempBullet);

        return false; //josh: had to return something


}

bool BulletHandler::DestroyBullet(int index){

        Bulletarray.erase(Bulletarray.begin()+index);

        return false; //josh: had to return something
}


vector<BulletCoord> BulletHandler::GetBullets(){
        return Bulletarray;
}

BulletCoord BulletHandler::GetBullet(int index){

        return Bulletarray.at(index);

}

int BulletHandler::NumBullets(){

        return Bulletarray.size();

}

void BulletHandler::BulletX(int index, float deltaX){

    BulletCoord tempCoord{Bulletarray.at(index).mX+deltaX,Bulletarray.at(index).mY,Bulletarray.at(index).flipped};
   //cout<<endl<<"Bullet "<<index<<" has initial coord "<<Bulletarray.at(index).mX<<"it is changed to"<<tempCoord.mX;
    Bulletarray.erase(Bulletarray.begin()+index);
    Bulletarray.insert(Bulletarray.begin()+index,tempCoord );

}
