# UtilityAI

A flexible and powerful Utility AI decision-making system for Unreal Engine. This plugin enables NPCs to make intelligent decisions based on weighted considerations and task scoring.

## Overview

UtilityAI is a runtime plugin that implements a utility-based AI decision system, commonly used in game AI to select between multiple behaviors based on weighted factors. Instead of traditional state machines or behavior trees, this system evaluates the "utility" or "desire" for each available task and executes the one with the highest score.

## Features

- **Utility-Based Task Selection**: Evaluate available tasks and automatically select the highest-scoring one
- **Consideration System**: Break down decision logic into reusable weighted considerations
- **Dynamic Task Management**: Add and remove tasks at runtime
- **Schedule System**: Define time-based task schedules through data assets
- **Blueprint Integration**: Full Blueprint support for easy prototyping and debugging
- **Extensible Architecture**: Easily create custom tasks and considerations
- **Interruption Support**: Tasks can be configured to allow or prevent interruption

## Architecture

### Core Components

#### **UUtilityAIComponent**
The main component that manages task evaluation and execution. Attach this to your AI-controlled character or pawn.

- Manages available tasks (both default and dynamic)
- Evaluates task scores each frame
- Handles scheduled tasks via data assets
- Executes tasks and manages their lifecycle

#### **UUtilityTask**
Represents a single decision option or behavior the AI can perform.

**Properties:**
- `Name`: Identifier for the task
- `Weight`: Base weight multiplier for scoring
- `Considerations`: Array of weighted considerations that affect the score
- `bCanExecute`: Whether the task is currently executable
- `bCanBeInterrupted`: Whether this task can be interrupted by a higher-scoring task

**Methods:**
- `CalculateScore()`: Computes the total utility score based on all considerations
- `ExecuteTask()`: Triggers the task execution (override in subclasses)
- `Initialize()`: Setup logic called when the task is created

#### **UUtilityConsideration**
An individual factor that influences a task's score. Create custom considerations by subclassing this class.

**Properties:**
- `Name`: Identifier for the consideration
- `Weight`: How much this consideration affects the task score

**Methods:**
- `Evaluate()`: Returns a 0-1 score representing how much this factor desires the task (implement in Blueprint or C++)

#### **UScheduleDataAsset**
A data asset that defines time-based task schedules. Use this to create recurring AI behaviors that activate at specific times.

**Properties:**
- `Schedules`: Array of time-based task definitions with start/end times

### Supporting Structures

**FUtilityContext**
- `AIController`: The controller managing this AI
- `Owner`: The actor owning the component
- `Target`: The current target/focus of the AI
(Used to pass contextual information to considerations and tasks)

**FUtilityTaskStruct**
- `Task`: The task class to instantiate
- `Weight`: Task weight multiplier
- `bCanExecute`: Execution permission
- `bCanBeInterrupted`: Interruption permission

**FScheduleStruct**
- `ID`: Schedule identifier
- `StartTime`: When the schedule begins
- `EndTime`: When the schedule ends
- `ScheduleTask`: The task to run during this schedule

## Getting Started

### Installation

1. Clone or download this plugin into your Unreal Engine project's `Plugins` folder
2. Rebuild your project
3. Enable the UtilityAI plugin in your project settings

### Basic Setup

#### 1. Create a Custom Task

**C++ Approach:**
```cpp
UCLASS()
class YOURGAME_API UPatrolTask : public UUtilityTask
{
    GENERATED_BODY()

public:
    virtual void ExecuteTask() override
    {
        // Implement patrolling logic
    }
};
```

**Blueprint Approach:**
- Right-click in Content Browser → Create Blueprint Based on → UUtilityTask
- Implement the execution logic in the Event Graph

#### 2. Create Custom Considerations

```cpp
UCLASS()
class YOURGAME_API UEnemyDistanceConsideration : public UUtilityConsideration
{
    GENERATED_BODY()

public:
    virtual float Evaluate_Implementation(FUtilityContext Context) override
    {
        // Return 0-1 based on enemy distance
        // Higher value = higher desire for the task
        return DesiredScore;
    }
};
```

#### 3. Attach Component to AI Character

1. Open your AI character's Blueprint
2. Add a **UtilityAIComponent** to the character
3. In the Details panel, configure:
   - **Default Tasks**: Add your task classes with their considerations
   - **Schedule**: (Optional) Assign a schedule data asset for time-based tasks

#### 4. Call Decision Making

In your AI logic (controller or pawn blueprint):
```cpp
// Find the best task to execute
UUtilityTask* SelectedTask = nullptr;
float Score = 0.0f;
FUtilityContext Context;
Context.AIController = MyAIController;
Context.Owner = MyCharacter;
Context.Target = MyTarget;

UtilityAIComponent->FindHighestScoreTask(Context, SelectedTask, Score);

if (SelectedTask)
{
    SelectedTask->ExecuteTask();
}
```

### Dynamic Task Management

Add or remove tasks at runtime:

```cpp
// Add a task dynamically
UtilityAIComponent->AddDynamicTask(UTakeCoverTask::StaticClass(), true);

// Remove it when done
UtilityAIComponent->RemoveDynamicTask(UTakeCoverTask::StaticClass());
```

### Using Schedules

1. Create a **Data Asset** of type `ScheduleDataAsset`
2. Add schedule entries with:
   - Unique ID
   - Start Time (in game time)
   - End Time (in game time)
   - Task configuration
3. Assign to the UtilityAIComponent's Schedule property
4. Activate specific schedules at runtime:

```cpp
UtilityAIComponent->AddScheduleTask(FName("PatrolSchedule"));
UtilityAIComponent->RemoveScheduleTask(FName("PatrolSchedule"));
```

## Scoring System

**How Task Scores are Calculated:**

1. For each consideration in a task:
   - `Evaluate()` returns a 0-1 score
   - Multiply by the consideration's Weight
2. Sum all weighted consideration scores
3. Multiply by the task's base Weight
4. The task with the highest final score is selected

**Example:**
```
Task Score = TaskWeight × Σ(ConsiderationWeight × ConsiderationScore)
```

If a task has multiple considerations, they're additive. This allows complex decision logic by combining simple evaluations.

## Blueprint Workflow

The system is fully Blueprint-compatible:

- Create custom tasks and considerations in Blueprint
- Set up default tasks and schedules in the component Details panel
- Call `FindHighestScoreTask` from Blueprint event graphs
- Handle `OnTaskCompleted` event to clean up or transition states

## Debugging

The component includes debug logging:
- On-screen messages when no tasks are available
- Task and consideration names for tracking decisions
- Inspect active tasks and their scores in the editor

## Example Use Cases

- **Patrol/Engage/Retreat**: Evaluate enemy distance and ammo to decide behavior
- **Resource Gathering**: Prioritize gathering tasks based on needs
- **Hierarchical AI**: Combine utility AI with higher-level planning systems
- **NPC Behavior**: Schedule daily routines with dynamic interruptions
- **Squad AI**: Different units decide actions independently based on context

## Performance Considerations

- Task evaluation happens on-demand via `FindHighestScoreTask()`
- Tick is disabled by default; call the function from your game loop
- Dynamic task creation uses object pooling (reuses existing instances)
- Considerations should be lightweight (avoid expensive queries)

## License

Created by Gavin Tsakponu

**Version:** 1.0  
**Unreal Engine Compatibility:** 5.x  
**Plugin Type:** Runtime Module
