#pragma once

class InputTrigger;

class InputTriggerAction
{
public:
   virtual void OnInput( InputTrigger& owner ) = 0;
   virtual void Clear( InputTrigger& owner ) = 0;
};

class InputTriggerDown : public InputTriggerAction
{
public:
   void OnInput( InputTrigger& owner ) override;
   void Clear( InputTrigger& owner ) override;
};

class InputTriggerUp : public InputTriggerAction
{
public:
   void OnInput( InputTrigger& owner ) override;
   void Clear( InputTrigger& owner ) override;
};

class NullInputTriggerAction : public InputTriggerAction
{
public:
   void OnInput( InputTrigger& owner ) override;
   void Clear( InputTrigger& owner ) override;
};
