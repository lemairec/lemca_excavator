#ifndef CUSTOM_CAP_NBR_H
#define CUSTOM_CAP_NBR_H

const size_t nbr = 10;

class CustomCapNbr {
public:
    double m_xs[nbr];
    double m_ys[nbr];
    size_t m_ind = 0;
    
    double m_custom_cap;
    double m_custom_cap_deg;
    
    double m_v_m_s;
    double m_v_km_h;
    
    CustomCapNbr(){
        for(size_t i = 0; i < nbr; ++i){
            m_xs[i] = 0;
            m_ys[i] = 0;
            m_ind = 0;
        }
    }
    
    void reset(){
        m_ind = 0;
    }
    
    void workXY(double x, double y){
        for(size_t i = nbr-1; i > 0; --i){
            m_xs[i] = m_xs[i-1];
            m_ys[i] = m_ys[i-1];
        }
        m_xs[0] = x;
        m_ys[0] = y;
        
        if(m_ind != 0){
            if(m_ind > nbr-1){
                m_ind = nbr-1;
            }
            
            double x_prev = m_xs[m_ind];
            double y_prev = m_ys[m_ind];
            
            m_custom_cap = atan2(x-x_prev, y-y_prev);
            m_custom_cap_deg = m_custom_cap/3.14*180;
            
            m_v_m_s = sqrt((x-x_prev)*(x-x_prev)+(y-y_prev)*(y-y_prev));
            m_v_km_h = m_v_m_s*3.6;
        }
        m_ind++;
    }
};

//regression lineaire
/*int n = m_ind;
if(n > 2){
    double sum_x = 0;
    double sum_y = 0;
    for(size_t i = 0; i < n; ++i){
        sum_x += m_xs[i];
        sum_y += m_ys[i];
    }
    double moy_x = sum_x/n;
    double moy_y = sum_y/n;
    
    double sum_sx2 = 0;
    double sum_sy2 = 0;
    double sum_sxy = 0;
    for(size_t i = 0; i < n; ++i){
        sum_sx2 += (m_xs[i]-moy_x)*(m_xs[i]-moy_x);
        sum_sy2 += (m_ys[i]-moy_y)*(m_ys[i]-moy_y);
        sum_sxy += (m_xs[i]-moy_x)*(m_ys[i]-moy_y);
    }
    double sx2 = sum_sx2/(n-1);
    double sy2 = sum_sy2/(n-1);
    double sxy = sum_sxy/(n-1);


    double beta1 = sxy/sx2;
    double beta2 = sxy/sy2;
    
    double a1 = atan(1/beta1);
    double a2 = atan(beta2);
    
    double a = atan2(x-x_prev, y-y_prev);
    
    m_custom_cap_deg2 = a/3.14*180;
    
    if(abs(a1-a) > 3.14/2){
        a1+=3.14;
    }
    if(abs(a2-a) > 3.14/2){
        a2+=3.14;
    }
    
    INFO(a1/3.14*180 << " " << a2/3.14*180)
   
    m_custom_cap = (a1+a2)/2;
}
m_custom_cap_deg = m_custom_cap/3.14*180;*/

#endif
