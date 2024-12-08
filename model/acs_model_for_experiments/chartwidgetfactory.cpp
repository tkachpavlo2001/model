#include "chartwidgetfactory.h"
#include "default_configuration_setter.h"
#include "QDebug"

iChartWidget::~iChartWidget() {}

iChartWidgetConfig::~iChartWidgetConfig() {}

iWidgetAbstractFactory::~iWidgetAbstractFactory() {}


void iChartWidget::_to_reset_chart()
{
    if (_pSeries != nullptr)
    {
        _pSeries->clear();
    }
}

void iChartWidget::_to_model_init()
{
    //_pRegistrator = Registrator_qt::to_new(this);
    _pRegistrator->to_set_series(_pSeries);

    const Default_configuration_setter default_configuration_setter_obj;
    // reference case
    ///_definder = std::make_shared<Reference_signal_definder_static>();
    ///_regulator = std::make_shared<PID_regulator>();
    _source = std::make_shared<DC_source_inerted>();
    _process = std::make_shared<DC_engine>();

    default_configuration_setter_obj.to_set_elements_parameters(
                _definder,
                _regulator,
                _source,
                _process
                );

    _process->to_set_calculation_mode(DC_engine::EULER);

    _acs_model = std::make_shared<Automated_control_system_paralleled>();
    _acs_model->to_mount_the_element(_definder.get());
    _acs_model->to_mount_the_element(_regulator.get());
    _acs_model->to_mount_the_element(_source.get());
    _acs_model->to_mount_the_element(_process.get());


    _experiment = std::make_shared<Experiment_executor>();
    _experiment->to_get_model_to_run(_acs_model.get());
    default_configuration_setter_obj.to_set_experiment_parameters(_experiment);
    //set

    double load = 20;
    double inertia = 0.05;
    auto arr = _process->to_get_parameters();
    arr[DC_engine::LOAD_K_0] = 1 * load;
    arr[DC_engine::LOAD_K_1] = 0 * load;
    arr[DC_engine::LOAD_K_EXP_CURVATURE] = 0 * load;
    arr[DC_engine::LOAD_K_EXP_LIMIT] = 0 * load;

    arr[DC_engine::MOMENT_OF_INERTIA_OF_MECHANICAL_LOAD] = 99 * inertia;
    _process->to_set_all_parameters(arr);

    _experiment->to_set_registrator(_pRegistrator);
    //alg
    ///_regulator->to_set_koefficients(3);
    ///_definder->to_set_signal(20);
    _source->to_receive_input_signal(20);

}


void ChartWidget_velocity::_to_run()
{
    ;
}

void ChartWidget_theta::_to_run()
{
    _to_reset_chart();


    _pAxisX->setTitleText("t");
    _pAxisY->setTitleText("theta(t)");
    _pMainChart->update();

    qDebug() << "DONE1\n";


    _experiment->to_set_registrator(_pRegistrator);
    _pRegistrator->to_set_dt_to_plot(_experiment->to_check_time_to_show());
    _experiment->to_run();

    qDebug() << "DONE0\n";
}

void ChartWidget_regulator::_to_run()
{
    ;
}
