import { ComponentFixture, TestBed } from '@angular/core/testing';

import { DoctorExaminationsComponent } from './doctor-examinations.component';

describe('DoctorExaminationsComponent', () => {
  let component: DoctorExaminationsComponent;
  let fixture: ComponentFixture<DoctorExaminationsComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ DoctorExaminationsComponent ]
    })
    .compileComponents();

    fixture = TestBed.createComponent(DoctorExaminationsComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
