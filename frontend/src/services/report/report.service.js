import jsreport from '@jsreport/browser-client';

jsreport.serverUrl = 'http://localhost:5488/';

async function ReportService() {
  try {
    // const parsedId = localStorage.getItem('idLine') || null;
    const report = await jsreport.render({
      template: {
        shortid: 'fV7ppDhwv',
      },
      data: {
        // idbusline: parsedId,
      },
    });

    report.openInWindow({ title: 'Informe de guaguas' });

    return report;
  } catch (error) {

    if (error && error.response && error.response.data) {
      console.error('Error al renderizar el informe:', error.response.data);
    } else {
      console.error('Error inesperado al renderizar el informe:', error);
      throw error;
    }
  }
}

// const ReportService = {
//   BusReportView
// };

export default ReportService;