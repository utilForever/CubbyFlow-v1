using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace CubbyFlowSharp.Tests
{
    [TestClass]
    public class UnitTest1
    {
        class MyAnimation : CubbyFlow.Animation
        {
            public CubbyFlow.Frame LastFrame;

            public void OnUpdate(CubbyFlow.Frame frame)
            {
                LastFrame = frame;
            }
        }

        [TestMethod]
        public void TestMethod1()
        {
            var f = new CubbyFlow.Frame(3, 0.02);
            var anim = new MyAnimation();

            anim.Update(f);

            Assert.AreEqual(anim.LastFrame.Index, 3);
            Assert.AreEqual(anim.LastFrame.TimeIntervalInSeconds, 0.02);
        }
    }
}
