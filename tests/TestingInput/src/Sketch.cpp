#include "Sketch.h"

using namespace std;
using namespace chr;

void Sketch::setup()
{}

void Sketch::draw()
{
  glClearColor(0.5f, 0.5f, 0.5f, 1);
  glClear(GL_COLOR_BUFFER_BIT);
}

void Sketch::keyPressed(uint32_t codepoint)
{
  LOGI << "keyPressed: " << codepoint << endl;
}

void Sketch::keyDown(int keyCode, int modifiers)
{
  string mods;
  if (modifiers & KeyEvent::MODIFIER_SHIFT) mods += " SHIFT";
  if (modifiers & KeyEvent::MODIFIER_CTRL) mods += " CTRL";
  if (modifiers & KeyEvent::MODIFIER_ALT) mods += " ALT";
  if (modifiers & KeyEvent::MODIFIER_META) mods += " META";

  LOGI << "keyDown: " << keyCode << " " << mods << endl;
}

void Sketch::keyUp(int keyCode, int modifiers)
{
  string mods;
  if (modifiers & KeyEvent::MODIFIER_SHIFT) mods += " SHIFT";
  if (modifiers & KeyEvent::MODIFIER_CTRL) mods += " CTRL";
  if (modifiers & KeyEvent::MODIFIER_ALT) mods += " ALT";
  if (modifiers & KeyEvent::MODIFIER_META) mods += " META";

  LOGI << "keyUp: " << keyCode << " " << mods << endl;
}

void Sketch::mouseMoved(float x, float y)
{
  LOGI << "mouseMoved: " << x << ", " << y << endl;
}

void Sketch::mouseDragged(int button, float x, float y)
{
  LOGI << "mouseDragged: " << button << " " << x << ", " << y << endl;
}

void Sketch::mousePressed(int button, float x, float y)
{
  LOGI << "mousePressed: " << button << " " << x << ", " << y << endl;
}

void Sketch::mouseReleased(int button, float x, float y)
{
  LOGI << "mouseReleased: " << button << " " << x << ", " << y << endl;
}

void Sketch::wheelUpdated(float offset)
{
  LOGI << "wheelUpdated: " << offset << endl;
}
