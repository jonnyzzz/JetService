using System;
using System.IO;
using System.Linq;
using System.Threading;
using System.Windows.Forms;

namespace JetService.IntegrationTests.WinForms
{
  public enum WinFormsTestAction
  {
    TEST_EMBEDDED_IE_OPEN,

    UNKNOWN
  }

  public static class TestWinFormsProgram
  {
    private static void WaitServiceToStart()
    {
      Thread.Sleep(TimeSpan.FromSeconds(5.5));
    }

    /// <summary>
    /// The main entry point for the application.
    /// </summary>
    [STAThread]
    static void Main(string[] args)
    {
      Application.EnableVisualStyles();
      Application.SetCompatibleTextRenderingDefault(false);
      
      WaitServiceToStart();
      
      var mainForm = new Form1();

      mainForm.AfterFormIsCreated =
        form =>
          {
            WinFormsTestAction result;
            if (!Enum.TryParse(args.FirstOrDefault(), true, out result))
              result = WinFormsTestAction.UNKNOWN;

            Console.Out.WriteLine("Executing action: {0}", result);


            switch (result)
            {
              case WinFormsTestAction.TEST_EMBEDDED_IE_OPEN:
                form.Browser.Navigated += delegate
                                            {
                                              File.WriteAllText(args[1], "I'm alive: " + form.Browser.DocumentText);
                                              form.Invoke((Action) form.Close);
                                            };
                form.Browser.Navigate("http://www.jetbrains.com");
                return;
              default:
                form.Close();

                return;
            }
          };

      Application.Run(mainForm);
    }
  }
}
