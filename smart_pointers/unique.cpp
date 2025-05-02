#include <cstddef>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>


class Song{
  public:
  std::wstring song;
  Song(){
    this->song = L""; 
  }
  Song(std::wstring song){
    this->song = song;
  }

  std::wstring showSong(){
    return this->song;
  }

};
void useRawPointer(){
  Song * song  = new Song(L"Nothing on you");

  std::wstring ss = song->showSong();
  std::wcout << ss << std::endl;
  delete song;
}

void smartThing(){

  std::unique_ptr<Song> song2(new Song(L"Nothing on you"));

  std::wstring s = song2->showSong();
  std::wcout << s << std::endl;

}


