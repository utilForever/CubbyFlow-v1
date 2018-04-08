using System.IO;
using CppSharp;
using CppSharp.AST;
using CppSharp.Generators;
using CppSharp.Parser;

namespace CubbyFlowSharp.CLI
{
    class CubbyFlow : ILibrary
    {
        public void Setup(Driver driver)
        {
            string path = Directory.GetCurrentDirectory();
            string includePath = System.IO.Path.Combine(path, @"../../../../../Includes");
            string boostIncludePath = System.IO.Path.Combine(path, @"../../../../../Libraries/boost");

            var driverOptions = driver.Options;
            driverOptions.GeneratorKind = GeneratorKind.CLI;

            var parserOptions = driver.ParserOptions;
            parserOptions.EnableRTTI = true;

            var module = driverOptions.AddModule("CubbyFlowSharp");
            module.IncludeDirs.Add(includePath);
            module.IncludeDirs.Add(boostIncludePath);

            module.Headers.Add("Core/Animation/Animation.h");
            module.Headers.Add("Core/Animation/Frame.h");
            module.Headers.Add("Core/Animation/PhysicsAnimation.h");

            module.Headers.Add("Core/Array/ArrayAccessor.h");
            module.Headers.Add("Core/Array/ArrayAccessor1.h");
            module.Headers.Add("Core/Array/ArrayAccessor1-Impl.h");
            module.Headers.Add("Core/Array/ArrayAccessor2.h");
            module.Headers.Add("Core/Array/ArrayAccessor2-Impl.h");
            module.Headers.Add("Core/Array/ArrayAccessor3.h");
            module.Headers.Add("Core/Array/ArrayAccessor3-Impl.h");

            module.Headers.Add("Core/Point/Point.h");
            module.Headers.Add("Core/Point/Point-Impl.h");
            module.Headers.Add("Core/Point/Point2.h");
            module.Headers.Add("Core/Point/Point2-Impl.h");
            module.Headers.Add("Core/Point/Point3.h");
            module.Headers.Add("Core/Point/Point3-Impl.h");

            module.Headers.Add("Core/Utils/Constants.h");

            module.LibraryDirs.Add(path);
            module.Libraries.Add("CubbyFlow.lib");
        }

        public void SetupPasses(Driver driver)
        {

        }

        public void Preprocess(Driver driver, ASTContext ctx)
        {

        }

        public void Postprocess(Driver driver, ASTContext ctx)
        {

        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            ConsoleDriver.Run(new CubbyFlow());
        }
    }
}
