#include "Sketch.h"

using namespace std;
using namespace chr;
using namespace gl;

Sketch::Sketch()
{}

void Sketch::setup()
{
  auto projectionMatrix = glm::ortho(0.0f, windowInfo.width, windowInfo.height, 0.0f);

  state
    .setShader(colorShader)
    .setShaderColor(1, 1, 1, 1)
    .setShaderMatrix(projectionMatrix);

  // ---

  glDisable(GL_DEPTH_TEST);
  glDepthMask(GL_FALSE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Sketch::draw()
{
  glClearColor(0.5f, 0.5f, 0.5f, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  state.apply();
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

  LOGI << "keyDown: " << keyCode << mods << endl;
}

void Sketch::keyUp(int keyCode, int modifiers)
{
  string mods;
  if (modifiers & KeyEvent::MODIFIER_SHIFT) mods += " SHIFT";
  if (modifiers & KeyEvent::MODIFIER_CTRL) mods += " CTRL";
  if (modifiers & KeyEvent::MODIFIER_ALT) mods += " ALT";
  if (modifiers & KeyEvent::MODIFIER_META) mods += " META";

  LOGI << "keyUp: " << keyCode << mods << endl;
}
