#include <noggit/UIDoodadSpawner.h>

#include <boost/filesystem.hpp>

#include <noggit/MapChunk.h>
#include <noggit/ModelManager.h> // ModelManager
#include <noggit/Noggit.h> // arial14
#include <noggit/UIButton.h>
#include <noggit/UITextBox.h>
#include <noggit/Video.h> // video
#include <noggit/WMOInstance.h> // WMOInstance
#include <noggit/World.h>

#include <noggit/Log.h>

// TODO : Add TreeView. Add ScrollBar. Add ModelPreview

static const float winWidth( 500.0f );
static const float winHeight( 100.0f );

static UIDoodadSpawner* global_doodadSpawner_evil( NULL );

void UIDoodadSpawner__TreeSelect( const std::string& value )
{
  global_doodadSpawner_evil->AddM2( value );
}

void UIDoodadSpawner__TextBoxEnter( UITextBox::Ptr textBox, const std::string& value )
{
  ( static_cast<UIDoodadSpawner *>( textBox->parent() ) )->AddM2( value );
}

void AddM2Click( UIFrame* f, int i )
{
  UITextBox::Ptr textBox( static_cast<UITextBox::Ptr>( f ) );
  ( static_cast<UIDoodadSpawner *>( textBox->parent() ) )->AddM2( textBox->value() );
}

extern std::list<std::string> gListfile;

UIDoodadSpawner::UIDoodadSpawner (World* world)
  : UICloseWindow( video.xres() / 2.0f - winWidth / 2.0f, video.yres() / 2.0f - winHeight / 2.0f, winWidth, winHeight, "Test", true )
  , _button( new UIButton( 145.0f, winHeight - 24.0f, 132.0f, 28.0f, "Test", "Interface\\Buttons\\UI-DialogBox-Button-Up.blp", "Interface\\Buttons\\UI-DialogBox-Button-Down.blp", AddM2Click, 0 ) )
  , _tbox( new UITextBox( 30.0f, 30.0f, 400.0f, 40.0f, UIDoodadSpawner__TextBoxEnter ) )
  , _treeView( UITreeView::Ptr() )
  , _world (world)
{
  global_doodadSpawner_evil = this;

  addChild( _button );
  addChild( _tbox );

  Directory::Ptr fileList( new Directory() );

  size_t found( std::string::npos );
  for( std::list<std::string>::const_iterator it( gListfile.begin() ), end( gListfile.end() ); it != end; ++it )
  {
    if( it->find( "m2" ) != std::string::npos )
    {
      found = it->find_last_of("/\\");
      if( found != std::string::npos )
        fileList->addDirectory( it->substr( 0, found ) )->addFile( it->substr( found + 1 ) );
      else
        fileList->addFile( *it );
    }
  }

  _treeView = UITreeView::Ptr( new UITreeView( 30.0f, 80.0f, "Models", fileList, UITreeView::Ptr(), UIDoodadSpawner__TreeSelect ) );
  addChild( _treeView.get() );
}

void UIDoodadSpawner::AddM2( const std::string& filename )
{
  Vec3D selectionPosition;

  switch( _world->GetCurrentSelection()->type )
  {
    case eEntry_Model:
      selectionPosition = _world->GetCurrentSelection()->data.model->pos;
      break;
    case eEntry_WMO:
      selectionPosition = _world->GetCurrentSelection()->data.wmo->pos;
      break;
    case eEntry_MapChunk:
      selectionPosition = _world->GetCurrentSelection()->data.mapchunk->GetSelectionPosition();
      break;
  }

  if( MPQFile::exists( filename ) )
  {
    std::string ext( boost::filesystem::extension( filename ) );
    std::transform( ext.begin(), ext.end(), ext.begin(), ::toupper );
    if(ext == ".M2")
    {
      _world->addM2( ModelManager::add( filename ), selectionPosition );
    }
    else if(ext == ".WMO")
    {
      _world->addWMO( WMOManager::add( _world, filename ), selectionPosition );
    }
  }
 }