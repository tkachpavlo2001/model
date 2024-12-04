#include "chartwidgetfactory.h"
#include "default_configuration_setter.h"

iChartWidget::~iChartWidget() {}

iChartWidgetConfig::~iChartWidgetConfig() {}

iWidgetAbstractFactory::~iWidgetAbstractFactory() {}

void ChartWidget_velocity::_to_run()
{
    ;
}

void ChartWidget_theta::_to_run()
{
    const Default_configuration_setter default_configuration_setter_obj;
    // reference case
    std::shared_ptr<Reference_signal_definder_static> definder = std::make_shared<Reference_signal_definder_static>();
    std::shared_ptr<PID_regulator> regulator = std::make_shared<PID_regulator>();
    std::shared_ptr<DC_source_inerted> source = std::make_shared<DC_source_inerted>();
    std::shared_ptr<DC_engine> process = std::make_shared<DC_engine>();

    default_configuration_setter_obj.to_set_elements_parameters(
                definder,
                regulator,
                source,
                process
                );

    process->to_set_calculation_mode(DC_engine::EULER);

    std::shared_ptr<Automated_control_system_paralleled> acs_model = std::make_shared<Automated_control_system_paralleled>();
    acs_model->to_mount_the_element(definder.get());
    acs_model->to_mount_the_element(regulator.get());
    acs_model->to_mount_the_element(source.get());
    acs_model->to_mount_the_element(process.get());


    std::shared_ptr<Experiment_executor_short_report> experiment = std::make_shared<Experiment_executor_short_report>();
    experiment->to_get_model_to_run(acs_model.get());
    experiment->to_set_result_title("Before the regulator tuning");
    default_configuration_setter_obj.to_set_experiment_parameters(experiment);
    //set

    double load = 20;
    double inertia = 0.05;
    auto arr = process->to_get_parameters();
    arr[DC_engine::LOAD_K_0] = 1 * load;
    arr[DC_engine::LOAD_K_1] = 0 * load;
    arr[DC_engine::LOAD_K_EXP_CURVATURE] = 0 * load;
    arr[DC_engine::LOAD_K_EXP_LIMIT] = 0 * load;

    arr[DC_engine::MOMENT_OF_INERTIA_OF_MECHANICAL_LOAD] = 99 * inertia;
    process->to_set_all_parameters(arr);
    //alg
    regulator->to_set_koefficients(1);
    definder->to_set_signal(20);
}

void ChartWidget_regulator::_to_run()
{
    ;
}
