/**************************************************************************************
*
*	Copyright (C) 2010 - 2012 Brent Andrew O. Chua - See LICENSE.txt for more info.
*		
*	Filename	-	Classes.h
*
*	Description -	Forward declaration of all classes in Pulse engine.
*
*	Comments	-	There's like a hundred more classes I need to include here but i'm
*					too lazy m'kay?
*
*	Modification History:
*			Name			   Date					Description
*			MrCodeSushi	-	05/29/2010		-	File creation
**************************************************************************************/

#ifndef _PSX_CLASSES_H_
#define _PSX_CLASSES_H_

#include "Platform.h"

namespace Pulse
{
	// Forward class declarations
	//////////////////////////////

	class Engine;

	// Application class
	class IApplication;
	class WindowsBaseApplication;
	class ConsoleApplication;
	class WindowApplication;

	// Pulse container classes
	template < typename T >
	class Array;
	template < typename T >
	class BinaryTree;
	template< typename K, typename T, typename Trait, typename Hash >
	class HashTable;
	template < typename T >
	class List;
	template < typename T >
	class LList;
	template < typename K, typename T, typename Trait >
	class Map;
	template < typename T, typename Trait >
	class PriorityQueue;
	template < typename T >
	class Queue;
	template < typename T, typename Trait >
	class Set;
	template < typename T, typename Trait >
	class SortedLList;
	template < typename T >
	class Stack;

	// Utility classes
	class Clock;
	class FPSCounter;
	class FPSAutoCounter;
	class String;
	class IRegistryObject;
	class Registry;
	class RegisterClassRAII;
	template < typename T >
	class SmartPointer;

	// I/O Classes
	class Stream;
	class IStream;
	class OStream;
	class FileIO;
	class XMLParser;
	class XMLElement;
	typedef SmartPointer<XMLElement> XMLElementPtr;

	// Math classes
	class Box;
	class FastFloat;
	class Math;
	class Matrix3x3;
	class Matrix4x4;
	class Random;
	class Vector2;
	class Vector3;
	class Vector4;

	// Systems/Manager classes
	class PulseScript;
	class ResHandle;
	class ResourceCache;
	class ResourceFile;
	class FileSystem;
	class ProcessManager;
	class EventManager;
	class InputSystem;
	class IDevice;
	class IKeyboardDevice;
	class IMouseDevice;
	class VertexBuffer;
	class TaskScheduler;

	// Threading
	class WorkerThread;
	class ThreadTask;
	class TaskCompletion;

	// Resource manager classes
	class IResourcePoolItem;
	template < typename T >
	class ResourcePool;
	class ResourceManager;

	// Graphics
	class GraphicsInfrastructure;
	class GraphicsDevice;
	struct RendererDesc;
	class Renderer;
	struct DisplayMode;
	class GraphicsPipeline;
	typedef Array<DisplayMode> DisplayModeList;

	// Graphics - Resources
	class GraphicsResource;
	class GraphicsBuffer;
	struct ConstantBufferDesc;
	class ConstantBuffer;
	struct VertexBufferDesc;
	class VertexBuffer;
	struct IndexBufferDesc;
	class IndexBuffer;
	struct TextureDesc;
	class Texture;	
	struct Texture2DDesc;
	class Texture2D;
	struct SwapChainDesc;
	class SwapChain;

	// Graphics - Shaders
	struct ShaderDesc;
	class Shader;
	class VertexShader;
	class HullShader;
	class DomainShader;
	class GeometryShader;
	class PixelShader;
	class ComputeShader;
	class RenderEffect;
	class RenderPass;

	// Graphics - Pipeline
	class GrpahicsPipeline;
	class ShaderStageState;
	struct MappedResource;
	class InputLayout;
	class InputAssemblerStage;
	class VertexShaderStage;
	class HullShaderStage;
	class TesellatorStage;
	class DomainShaderStage;
	class GeometryShaderStage;
	class StreamOutputStage;
	class RasterizerStage;
	class PixelShaderStage;
	class OutputMergerStage;
	
	// Graphics - Parameter System
	class ParameterSystem;
	class IParameter;
	class ParameterMatrix;
	class ParameterMatrixArray;
	class ParameterShaderResource;
	class ParameterVector;
	class IParameterBinder;
	class ParameterBinderMatrix;
	class ParameterBinderMatrixArray;
	class ParameterBinderShaderResource;
	class ParameterBinderVector;
	
	// Graphics -  States
	class BlendState;
	class DepthStencilState;
	class RasterizerState;
	class SamplerState;

	// Graphics - Rendering
	class Geometry;
	class RenderModel;


	// Multi-threading
	template < BOOL bTry >
	class _MutexLocker;
	typedef _MutexLocker< TRUE > MutexLocker;
	typedef _MutexLocker< FALSE > MutexTryLocker;
	class Thread;

	// Application
	class Logic;
	class View;
	struct AppMsg;
	typedef UINT ActorID;
	class Actor;
	class Component;
	typedef UINT ComponentID;
	typedef SmartPointer<Actor> ActorPtr;
	typedef SmartPointer<Component> ActorComponentPtr;

}

#endif /* _PSX_CLASSES_H_ */