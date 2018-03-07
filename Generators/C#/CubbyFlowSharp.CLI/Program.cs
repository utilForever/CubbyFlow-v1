using System.IO;
using CppSharp;
using CppSharp.AST;
using CppSharp.Generators;

namespace CubbyFlowSharp.CLI
{
    class CubbyFlow : ILibrary
    {
        public void Setup(Driver driver)
        {
            string path = Directory.GetCurrentDirectory();
            string includePath = System.IO.Path.Combine(path, @"../../../../../Includes");

            var options = driver.Options;
            options.GeneratorKind = GeneratorKind.CSharp;

            var module = options.AddModule("CubbyFlowSharp");
            module.IncludeDirs.Add(includePath);
            module.Headers.Add("Core/Animation/Animation.h");
            module.Headers.Add("Core/Animation/Frame.h");
            module.Headers.Add("Core/Animation/PhysicsAnimation.h");
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
